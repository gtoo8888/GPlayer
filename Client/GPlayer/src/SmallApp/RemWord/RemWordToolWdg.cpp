#include "RemWordToolWdg.h"
#include <QFile>
#include <QFileDialog>
#include <QMessagebox>
#include <QTextStream>

RemWordToolWdg::RemWordToolWdg(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::RemWordToolWdg),
      mEnglishTotalLen(40),
      mProgressBarMin(0),
      mProgressBarMax(100),
      msOpenPath(
          R"(E:\Desktop\languguetest\CplusplusProject\GPlayer\Client\GPlayer\src\SmallApp\RemWord\DailyWords\20241217)"),
      msTransformFilePath(std::string("")),
      msTransformOutputFilePath(std::string("output.txt.word")),
      msImportFilPath(std::string("")),
      msImportOutputFilePath(std::string("output.md")) {
    ui->setupUi(this);

    ui->progressBar->setMinimum(mProgressBarMin);
    ui->progressBar->setMaximum(mProgressBarMax);
    ui->progressBar->setValue(mProgressBarMin);

    connect(ui->btnTransformFile, &QPushButton::clicked, this, &RemWordToolWdg::slotBtnOpenFileTransform);
    connect(ui->btnTransform, &QPushButton::clicked, this, &RemWordToolWdg::slotBtnTransform);
    connect(ui->btnImportFile, &QPushButton::clicked, this, &RemWordToolWdg::slotBtnOpenFileImport);
    connect(ui->btnImport, &QPushButton::clicked, this, &RemWordToolWdg::slotBtnImport);
}

RemWordToolWdg::~RemWordToolWdg() {
}

void RemWordToolWdg::slotBtnOpenFileTransform(void) {
    cleanWdg();
    vsMergeWord.clear();
    vsEnglishWord.clear();
    vsChineseWord.clear();
    QString transformFilePath = QFileDialog::getOpenFileName(
        this, "Selecting Source File", QString::fromStdString(msOpenPath), "WordList(*.txt.word);; Other(*)");
    if (transformFilePath.isEmpty()) {
        LOG_WRN("No transform file path");
        return;
    }
    ui->leTransform->setText(transformFilePath);
    msTransformFilePath = transformFilePath.toStdString();
    LOG_INF("Read transform file path: {}", msTransformFilePath);

    // TODO 是否考虑输入输出参数
    if (!readFileDatafstream(msTransformFilePath)) {
        QMessageBox::warning(this, "警告", "警告！中英文单词数量不相等");
        return;
    }
    showEnglishWordTxt();
}

void RemWordToolWdg::slotBtnTransform(void) {
    cleanWdg();
    vsMergeWord.clear();
    QString transformOutputFilePath = QFileDialog::getSaveFileName(
        this, "Selecting Source Output File", QString::fromStdString(msOpenPath), "WordList(*.txt.word);; Other(*)");
    if (transformOutputFilePath.isEmpty()) {
        LOG_WRN("No transform file path");
        return;
    }

    // QFileInfo outputFileInfo(transformOutputFilePath);
    // QString fileName, fileSuffix, absolutePath;
    // fileName = outputFileInfo.fileName(); // 文件名
    // fileSuffix = outputFileInfo.suffix(); // 文件后缀
    // absolutePath = outputFileInfo.absolutePath(); // 绝对路径
    // msTransformOutputFilePath = transformOutputFilePath.toStdString();

    // TOTEST
    // msTransformFilePath = R"(E:\Desktop\languguetest\CplusplusProject\GPlayer\Client\GPlayer\input.txt.word)";
    // msTransformOutputFilePath = R"(E:\Desktop\languguetest\CplusplusProject\GPlayer\Client\GPlayer\ouput.txt.word)";
    msTransformOutputFilePath = transformOutputFilePath.toStdString();
    LOG_INF("Transform outpur file path: {}", msTransformOutputFilePath);
    if (msTransformOutputFilePath == msTransformFilePath) {
        LOG_WRN("Output file path is the same as the input file path: {}", msTransformOutputFilePath);
        // std::string warnSamePath("警告！输出文件路径和输入文件路径相同！");
        // ui->textBrowser->append(QString(QString::fromLocal8Bit(warnSamePath.c_str())));
        QMessageBox::warning(this, "警告", "警告！输出文件路径和输入文件路径相同");
        return;
    }

    ui->progressBar->setValue(mProgressBarMax * 0.3);
    if (!mergeChineseAndEnglishWord()) {
        return;
    }
    ui->progressBar->setValue(mProgressBarMax * 0.6);
    if (!writeMergeWord(msTransformOutputFilePath)) {
        return;
    }
    ui->progressBar->setValue(mProgressBarMax * 1);
    showMergeWord();
}

void RemWordToolWdg::slotBtnOpenFileImport(void) {
    cleanWdg();
    QString importFilePath = QFileDialog::getOpenFileName(
        this, "Selecting Import file", QString::fromStdString(msOpenPath), "WordList(*.txt.word);; Other(*)");
    if (importFilePath.isEmpty()) {
        LOG_WRN("No import file path");
        return;
    }
    ui->leImportPath->setText(importFilePath);
    msImportFilPath = importFilePath.toStdString();
    LOG_INF("Read import file path: {}", msImportFilPath);

    if (!readImportFileDatafgets(msImportFilPath)) {
        return;
    }
    showPreImportTxt();
}

void RemWordToolWdg::slotBtnImport(void) {
    cleanWdg();
    // TODO 考虑这个结构的内存释放
    rootWordTree = std::make_shared<WordTreeNode>("root", false);
    // QString importOutputFilePath = QFileDialog::getOpenFileName(
    //     this, "Selecting Import file", QString::fromStdString(msOpenPath), "Markdown (*.md);; Other(*)");
    // if (importOutputFilePath.isEmpty()) {
    //     LOG_WRN("No import file path");
    //     return;
    // }

    //---TOTEST
    msImportFilPath =
        R"(E:\Desktop\languguetest\CplusplusProject\GPlayer\Client\GPlayer\src\SmallApp\RemWord\DailyWords\20241217\ouput.txt.word)";
    LOG_INF("Read import file path: {}", msImportFilPath);
    if (!readImportFileDatafgets(msImportFilPath)) {
        return;
    }
    showPreImportTxt();
    msImportOutputFilePath =
        R"(E:\Desktop\languguetest\CplusplusProject\GPlayer\Client\GPlayer\src\SmallApp\RemWord\DailyWords\20241217\1217.md)";
    //---TOTEST

    // msImportOutputFilePath = importOutputFilePath.toStdString();
    LOG_INF("Import file output path: {}", msImportOutputFilePath);

    if (!parseInfoFromPreImport()) {
        return;
    }
    if (!initOutputFilename(msImportOutputFilePath)) {
        return;
    }
    if (!processWordTree2Vector()) {
        return;
    }
    if (!writeWordMd()) {
        return;
    }
}

bool RemWordToolWdg::readFileDatafstream(std::string filePath) {
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

        std::string::iterator end_pos = std::remove(inputLine.begin(), inputLine.end(), ' ');
        inputLine.erase(end_pos, inputLine.end());
        // LOG_DBG("{} isLetterC:{}", inputLine, isLetterC(inputLine[0]));
        if (isLetterC(inputLine[0])) {
            vsEnglishWord.push_back(inputLine);
        } else {
            vsChineseWord.push_back(inputLine);
        }
    }
    LOG_INF("vsEnglishWord:{} vsChineseWord:{}", vsEnglishWord.size(), vsChineseWord.size());
    if (vsEnglishWord.size() != vsChineseWord.size()) {
        LOG_WRN("English and Chinese Word not Equal");
        return false;
    }
    LOG_INF("Read File Data finish");
    inputFile.close();
    return true;
}

bool RemWordToolWdg::readFileDatafgets(std::string filePath) {
    FILE* fd = fopen(filePath.c_str(), "r");
    if (fd == NULL) {
        LOG_ERR("Open fail errno = %d reason = %s \n", errno, strerror(errno));
        return false;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fd)) {  // 不会自动去除\n
        LOG_DBG("{}", line);
    }

    fclose(fd);
    return true;
}

bool RemWordToolWdg::readFileDataQFile(std::string filePath) {
    QFile aFile(QString::fromStdString(filePath));
    if (!aFile.exists()) {
        return false;
    }
    // 如果不是以只读和文本方式打开文件返回false
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&aFile);
    in.setCodec("UTF-8");  // 或者 "GB18030" 对于兼容GBK的中文编码

    QString content = in.readAll();
    aFile.close();
    return true;
}

bool RemWordToolWdg::isLetterC(char value) {
    if (value >= 'a' && value <= 'z') {
        return true;
    } else if (value >= 'A' && value <= 'Z') {
        return true;
    }
    return false;
}

bool RemWordToolWdg::isLetterCplusplus(char value) {
    return std::isalpha(static_cast<unsigned char>(value)) != 0;
}

bool RemWordToolWdg::mergeChineseAndEnglishWord(void) {
    int wordLen = vsChineseWord.size();
    vsMergeWord.resize(wordLen);
    for (int i = 0; i < wordLen; i++) {
        int fillBlankNum = mEnglishTotalLen - vsEnglishWord[i].size();
        if (fillBlankNum < 0) {
            LOG_WRN("The word {} length is over {}", vsEnglishWord[i], mEnglishTotalLen);
            continue;
        }
        // TODO 字符串拼接是否可以效率优化？
        // TODO 效率提升多少？
        std::string wordMerge = vsEnglishWord[i] + std::string(fillBlankNum, ' ') + "| " + vsChineseWord[i];
        // LOG_DBG("idx:{} {}", i, wordMerge);
        vsMergeWord[i] = wordMerge;
    }
    LOG_INF("Merge chinese and english word finsh");
    return true;
}

bool RemWordToolWdg::writeMergeWord(std::string filePath) {
    std::ofstream outputFile(filePath, std::ifstream::out | std::ifstream::trunc);
    if (!outputFile.is_open()) {
        return false;
    }

    outputFile << "# Title\n";
    outputFile << "## Sub Title\n";
    for (const std::string& word : vsMergeWord) {
        outputFile << word << "\n";
    }

    if (outputFile.fail()) {
        return false;
    }

    outputFile.close();
    LOG_INF("Write Merge Word finsh");
    return true;
}

bool RemWordToolWdg::showEnglishWordTxt(void) {
    int wordLen = vsChineseWord.size();
    for (int i = 0; i < wordLen; i++) {
        ui->textBrowser->append(QString::fromLocal8Bit(vsEnglishWord[i].c_str()));
        ui->textBrowser->append(QString::fromLocal8Bit(vsChineseWord[i].c_str()));
    }
    return true;
}

bool RemWordToolWdg::showMergeWord(void) {
    ui->textBrowser->append(QString("# Title"));
    ui->textBrowser->append(QString("## Sub Title"));
    int wordLen = vsMergeWord.size();
    for (std::string& str : vsMergeWord) {
        // 只有这种方式string转QString中文不会乱码
        const QString qstringMergeWord = QString::fromLocal8Bit(str.c_str());
        ui->textBrowser->append(qstringMergeWord);
    }
    return true;
}

bool RemWordToolWdg::cleanWdg(void) {
    QString browserText = ui->textBrowser->toPlainText();
    if (browserText != QString("")) {
        ui->textBrowser->clear();
    }
    return true;
}

bool RemWordToolWdg::readImportFileDatafgets(std::string filePath) {
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

bool RemWordToolWdg::showPreImportTxt(void) {
    int wordLen = vsPreImportWord.size();
    for (int i = 0; i < wordLen; i++) {
        ui->textBrowser->append(QString::fromLocal8Bit(vsPreImportWord[i].c_str()));
    }
    return true;
}

bool RemWordToolWdg::initOutputFilename(std::string filePath) {
    std::string fnWithoutExt = getFilenameWithoutExe(filePath);
    std::string fileSuffix = getFileSuffix(filePath);
    std::string absolutePath = getAbsolutePath(filePath);

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

bool RemWordToolWdg::parseInfoFromPreImport(void) {
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
            splitStringAtDelimiter(nowLine, '|', leftPart, rightPart);
            WordInfo wordInfo(wordNumInCharpter, leftPart, rightPart);
            subTitle->addWord(wordInfo);
        }
    }
    return true;
}

bool RemWordToolWdg::splitStringAtDelimiter(const std::string& str, char delimiter, std::string& left,
                                            std::string& right) {
    size_t delimiterPos = str.find(delimiter);
    if (delimiterPos != std::string::npos) {
        // 提取左边部分，并去除前导和尾随空格
        left = str.substr(0, delimiterPos);
        left.erase(0, left.find_first_not_of(" \t\n\r\f\v"));  // 去除前导空格
        left.erase(left.find_last_not_of(" \t\n\r\f\v") + 1);  // 去除尾随空格

        // 提取右边部分，并去除前导和尾随空格
        right = str.substr(delimiterPos + 1);
        right.erase(0, right.find_first_not_of(" \t\n\r\f\v"));  // 去除前导空格
        right.erase(right.find_last_not_of(" \t\n\r\f\v") + 1);  // 去除尾随空格
        return true;
    } else {
        // 如果没有找到分隔符，则原字符串为左部分，右部分为空
        left = str;
        right.clear();
        return false;
    }
    return true;
}

std::string RemWordToolWdg::wordInfo2String(WordInfo wordInfo) {
    int fillBlankNum = mEnglishTotalLen - wordInfo.english.size();
    if (fillBlankNum < 0) {
        LOG_WRN("The word {} length is over {}", wordInfo.english, mEnglishTotalLen);
        return "";
    }
    char buf[4];
    sprintf(buf, "%03d. ", wordInfo.idx);
    std::string mergeWord =
        std::string(buf) + wordInfo.english + std::string(fillBlankNum, ' ') + "| " + wordInfo.chinese;
    LOG_DBG("mergeWord:{}", mergeWord);

    return mergeWord;
}

bool RemWordToolWdg::processWordTree2Vector(void) {
    std::vector<std::shared_ptr<WordTreeNode>> mainTitle = rootWordTree->subTitle;
    std::string tmpTitle;
    for (std::shared_ptr<WordTreeNode> item : mainTitle) {
        tmpTitle = std::string("# ") + std::string(item->title);
        vsOChAndEnWord.push_back(tmpTitle);
        for (std::shared_ptr<WordTreeNode> it : item->subTitle) {
            char bufWordNum[5];
            sprintf(bufWordNum, " (%d)", it->words.size());
            tmpTitle = std::string("## ") + std::string(it->title) + std::string(bufWordNum);
            vsOChAndEnWord.push_back(std::string(tmpTitle));
            for (WordInfo word : it->words) {
                std::string mergeWord = wordInfo2String(word);
                vsOChAndEnWord.push_back(mergeWord);
            }
        }
    }
    return true;
}

bool RemWordToolWdg::writeWordMd(void) {
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

bool RemWordToolWdg::writeLanguagueMd(std::string path, LanguageType type) {
    std::ofstream outputFile(path, std::ifstream::out | std::ifstream::trunc);
    if (!outputFile.is_open()) {
        LOG_ERR("Open fail errno = %d reason = %s \n", errno, strerror(errno));
        return false;
    }
    std::vector<std::string> oWord;
    switch (type) {
        case Chinese:
            outputFile << "# 单独中文\n";
            oWord = vsOChineseWord;
            break;
        case English:
            outputFile << "# 单独英文\n";
            oWord = vsOEnglishWord;
            break;
        case ChAndEn:
            outputFile << "# 中英文混合\n";
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

std::string RemWordToolWdg::getFilename(const std::string& path) {
    size_t pos = path.find_last_of("/\\");

    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    // 如果没有找到斜杠，说明整个字符串就是文件名
    return path;
}

std::string RemWordToolWdg::getFilenameWithoutExe(const std::string& path) {
    size_t namePos = path.find_last_of("/\\");

    std::string filename;
    if (namePos != std::string::npos) {
        filename = path.substr(namePos + 1);
    } else {
        filename = path;
    }

    size_t dosPos = filename.find_last_of(".");
    if (dosPos != std::string::npos && dosPos > 0) {
        return filename.substr(0, dosPos);
    }
    // 如果没有找到点号或者点号是第一个字符，返回原始文件名
    return filename;
}

std::string RemWordToolWdg::getFileSuffix(const std::string& path) {
    size_t namePos = path.find_last_of("/\\");

    std::string filename;
    if (namePos != std::string::npos) {
        filename = path.substr(namePos + 1);
    } else {
        filename = path;
    }

    size_t dosPos = filename.find_last_of(".");
    if (dosPos != std::string::npos && dosPos > 0) {
        return filename.substr(dosPos);
    }
    return "";
}

std::string RemWordToolWdg::getAbsolutePath(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos) {
        return path.substr(0, pos);
    }
    return path;
}
