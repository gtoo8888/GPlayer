#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include "RemWordToolWdg.h"

WordTranform::WordTranform()
    : mEnglishTotalLen(40) {
}

bool WordTranform::readFileDatafstream(std::string filePath) {
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
        if (GUtils::isLetterC(inputLine[0])) {
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

bool WordTranform::readFileDatafgets(std::string filePath) {
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

bool WordTranform::readFileDataQFile(std::string filePath) {
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

bool WordTranform::mergeChineseAndEnglishWord(void) {
    size_t wordLen = vsChineseWord.size();
    vsMergeWord.resize(wordLen);
    for (size_t i = 0; i < wordLen; i++) {
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

bool WordTranform::writeMergeWord(std::string filePath) {
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
