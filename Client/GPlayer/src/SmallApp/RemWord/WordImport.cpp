#include "RemWordToolWdg.h"

WordImport::WordImport()
    : mEnglishTotalLen(40) {
}

bool WordImport::readImportFileDatafgets(std::string filePath) {
    std::ifstream inputFile;
    inputFile.open(filePath, std::fstream::in);
    if (!inputFile.is_open()) {
        LOG_ERR("Can't open file: {}", filePath);
        if (inputFile.rdstate() & std::ifstream::failbit) {
            LOG_ERR("Formatting input/output failed or file not found");
        }
        if (inputFile.rdstate() & std::ifstream::badbit) {
            LOG_ERR("Read/write error, possibly a hardware problem or file corruption");
        }
        LOG_ERR("({})", strerror(errno));
        return false;
    }

    std::string inputLine;
    while (std::getline(inputFile, inputLine)) {
        if (inputLine.size() == 0) {
            LOG_INF("Is Blank line");
            continue;
        }

        // 空格要小心处理，中间的都不要去掉，是格式化的空格
        // std::string::iterator end_pos = std::remove(inputLine.begin(), inputLine.end(), ' ');
        // inputLine.erase(end_pos, inputLine.end());
        // LOG_DBG("{}", inputLine);
        vsPreImportWord.push_back(inputLine);
    }
    inputFile.close();
    LOG_INF("Read Import File Data finish");

    return true;
}

bool WordImport::parseInfoFromPreImport(void) {
    int preImportLen = vsPreImportWord.size();
    int chapterNum = 0;
    int wordNumInCharpter = 0;
    std::shared_ptr<WordTreeNode> mainTitle = nullptr;
    std::shared_ptr<WordTreeNode> subTitle = nullptr;
    for (int i = 0; i < preImportLen; i++) {
        std::string nowLine = vsPreImportWord[i];
        std::string startStr = nowLine.substr(0, 2);
        if (startStr == "# ") {
            wordNumInCharpter = 0;
            std::string mainTitleStr = nowLine.substr(2, nowLine.size() - 2);
            mainTitle = std::make_shared<WordTreeNode>(mainTitleStr, false);
            rootWordTree->addSubTitle(mainTitle);
        } else if (startStr == "##") {
            wordNumInCharpter = 0;
            std::string subTitleStr = nowLine.substr(3, nowLine.size() - 3);
            subTitle = std::make_shared<WordTreeNode>(subTitleStr, true);
            mainTitle->addSubTitle(subTitle);
        } else {
            wordNumInCharpter++;
            std::string leftPart, rightPart;
            GUtils::splitStringAtDelimiter(nowLine, '|', leftPart, rightPart);
            WordInfo wordInfo(wordNumInCharpter, leftPart, rightPart);
            subTitle->addWord(wordInfo);
        }
    }
    return true;
}

bool WordImport::initOutputFilename(std::string filePath) {
    std::string fnWithoutExt = GUtils::getFilenameWithoutExe(filePath);
    std::string fileSuffix = GUtils::getFileSuffix(filePath);
    std::string absolutePath = GUtils::getAbsolutePath(filePath);

    oFnChinese = absolutePath + "\\" + fnWithoutExt + "_cn" + fileSuffix;
    oFnEnglish = absolutePath + "\\" + fnWithoutExt + "_en" + fileSuffix;
    oFnCnAdnEn = absolutePath + "\\" + fnWithoutExt + "_en_cn" + fileSuffix;

    FILE* fdCn = fopen(oFnChinese.c_str(), "w+");
    if (fdCn == NULL) {
        LOG_ERR("Open fail errno = %d reason = %s \n", errno, strerror(errno));
        return false;
    }
    fclose(fdCn);
    FILE* fdEn = fopen(oFnEnglish.c_str(), "w+");
    if (fdEn == NULL) {
        LOG_ERR("Open fail errno = %d reason = %s \n", errno, strerror(errno));
        return false;
    }
    fclose(fdEn);
    FILE* fdCnAdnEn = fopen(oFnCnAdnEn.c_str(), "w+");
    if (fdCnAdnEn == NULL) {
        LOG_ERR("Open fail errno = %d reason = %s \n", errno, strerror(errno));
        return false;
    }
    fclose(fdCnAdnEn);

    return true;
}

bool WordImport::processWordTree2Vector(void) {
    std::vector<std::shared_ptr<WordTreeNode>> mainTitle = rootWordTree->subTitle;
    std::string tmpTitle;

    for (std::shared_ptr<WordTreeNode> item : mainTitle) {
        tmpTitle = std::string("# ") + item->title;
        vsOChAndEnWord.push_back(tmpTitle);
        vsOChineseWord.push_back(tmpTitle);
        vsOEnglishWord.push_back(tmpTitle);
        for (std::shared_ptr<WordTreeNode> it : item->subTitle) {
            char bufWordNum[5];
            sprintf(bufWordNum, " (%d)", it->words.size());
            tmpTitle = std::string("## ") + it->title + std::string(bufWordNum);
            vsOChAndEnWord.push_back(tmpTitle);
            vsOChineseWord.push_back(tmpTitle);
            vsOEnglishWord.push_back(tmpTitle);
            for (WordInfo word : it->words) {
                vsOChAndEnWord.push_back(wordInfo2String(word, LanguageType::ChAndEn));
                vsOChineseWord.push_back(wordInfo2String(word, LanguageType::Chinese));
                vsOEnglishWord.push_back(wordInfo2String(word, LanguageType::English));
                vsWordInfo.push_back(word);
            }
        }
    }
    return true;
}

std::string WordImport::wordInfo2String(WordInfo wordInfo, LanguageType type) {
    char buf[4];
    sprintf(buf, "%03d. \0", wordInfo.idx);
    std::string mergeWord;
    if (type == Chinese) {
        mergeWord = std::string(buf) + wordInfo.chinese;
    } else if (type == English) {
        mergeWord = std::string(buf) + wordInfo.english;
    } else if (type == ChAndEn) {
        int blankNum = mEnglishTotalLen - wordInfo.english.size();
        // TODO 未知原因，只要这个判断开启，就有乱码加入，其中什么都不做，也会乱码
        // if (blankNum > 0) {
        //    // LOG_WRN("The word {} length is over {}", wordInfo.english, mEnglishTotalLen);
        //    // return "";
        //}
        mergeWord = std::string(buf) + wordInfo.english + std::string(blankNum, ' ') + "| " + wordInfo.chinese;
        // mergeWord = std::string(buf) + wordInfo.english;
        LOG_DBG("mergeWord:{}", mergeWord);
    }

    return mergeWord;
}

bool WordImport::writeWordMd(void) {
    bool isWriteOk = true;
    if (!writeLanguagueMd(oFnChinese, LanguageType::Chinese)) {
        isWriteOk = false;
    }
    if (!writeLanguagueMd(oFnEnglish, LanguageType::English)) {
        isWriteOk = false;
    }
    if (!writeLanguagueMd(oFnCnAdnEn, LanguageType::ChAndEn)) {
        isWriteOk = false;
    }
    if (!isWriteOk) {
        LOG_ERR("Write Wort to Markdown Fail");
        return false;
    }
    return true;
}
bool WordImport::writeLanguagueMd(std::string path, LanguageType type) {
    std::ofstream outputFile(path, std::ifstream::out | std::ifstream::trunc);
    if (!outputFile.is_open()) {
        LOG_ERR("Open fail errno = {} reason = {} \n", errno, strerror(errno));
        return false;
    }
    std::vector<std::string> oWord;
    // ofstream 不能使用裸的中文输入，会直接将将破坏整个文件的编码格式
    switch (type) {
        case Chinese:
            // outputFile << "# 单独中文\n";
            outputFile << "# Only Chinese\n";
            oWord = vsOChineseWord;
            break;
        case English:
            // outputFile << "# 单独英文\n";
            outputFile << "# Only English\n";
            oWord = vsOEnglishWord;
            break;
        case ChAndEn:
            // outputFile << "# 中英文混合\n";
            outputFile << "# Mixed Chinese and English\n";
            oWord = vsOChAndEnWord;
            break;
        default:
            break;
    }

    for (const std::string& line : oWord) {
        outputFile << line << "\n";
    }

    if (outputFile.fail()) {
        return false;
    }
    outputFile.close();
    return true;
}

bool WordImport::saveWord2Sqlite(void) {
    mspWordSql = std::make_shared<WordSql>("word.db", "WordList");
    if (!mspWordSql->initDB()) {
        return false;
    }
    if (!mspWordSql->createWordTable()) {
        return false;
    }
    int wordNum = vsWordInfo.size();
    for (int i = 0; i < wordNum; i++) {
        WordSqlInfo wordSqlInfo(vsWordInfo[i].english, vsWordInfo[i].chinese);
        if (!mspWordSql->insertWord(wordSqlInfo)) {
            LOG_ERR("insert {} error", vsWordInfo[i].english);
        }
    }
    mspWordSql->closeDB();

    return true;
}

bool WordImport::testWord2Sqlite(void) {
    mspWordSql = std::make_shared<WordSql>("word.db", "WordList");
    if (!mspWordSql->initDB()) {
        return false;
    }
    if (!mspWordSql->testDB()) {
        return false;
    }
    return false;
}