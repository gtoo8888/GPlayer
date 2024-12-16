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
        this, "Selecting Source File", "E:/Desktop/languguetest/CplusplusProject/GPlayer/Client/GPlayer",
        "WordList(*.txt.word);; Other(*)");
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
        this, "Selecting Source Output File", "E:/Desktop/languguetest/CplusplusProject/GPlayer/Client/GPlayer",
        "WordList(*.txt.word);; Other(*)");
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
        //std::string warnSamePath("警告！输出文件路径和输入文件路径相同！");
        //ui->textBrowser->append(QString(QString::fromLocal8Bit(warnSamePath.c_str())));
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
    QString importFilePath = QFileDialog::getOpenFileName(
        this, "Selecting Import file", "E:/Desktop/languguetest/CplusplusProject/GPlayer/Client/GPlayer",
        "WordList(*.txt.word);; Other(*)");
    if (importFilePath.isEmpty()) {
        LOG_WRN("No import file path");
        return;
    }
    ui->leImportPath->setText(importFilePath);
    msImportFilPath = importFilePath.toStdString();
    LOG_WRN("Read import file path: {}", msImportFilPath);
}

void RemWordToolWdg::slotBtnImport(void) {
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
    for (int i = 0; i < wordLen;i++) {
        const QString qstringEnglishWord = QString(QString::fromLocal8Bit(vsEnglishWord[i].c_str()));
        ui->textBrowser->append(qstringEnglishWord);
        const QString qstringChineseWord = QString(QString::fromLocal8Bit(vsChineseWord[i].c_str()));
        ui->textBrowser->append(qstringChineseWord);
    }
    return true;
}

bool RemWordToolWdg::showMergeWord(void) {
    int wordLen = vsMergeWord.size();
    for (std::string& str : vsMergeWord) {
        // 只有这种方式string转QString中文不会乱码
        const QString qstringMergeWord = QString(QString::fromLocal8Bit(str.c_str()));
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
