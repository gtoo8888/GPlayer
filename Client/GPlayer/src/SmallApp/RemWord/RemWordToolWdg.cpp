#include "RemWordToolWdg.h"
#include <QFile>
#include <QFileDialog>
#include <QMessagebox>
#include <QTextStream>

RemWordToolWdg::RemWordToolWdg(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::RemWordToolWdg),
      mProgressBarMin(0),
      mProgressBarMax(100),
      msOpenPath(R"(E:\Desktop\languguetest\CplusplusProject\DailyWords)"),
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

    mspWordTranform = std::make_shared<WordTranform>();
    mspWordImport = std::make_shared<WordImport>();
}

RemWordToolWdg::~RemWordToolWdg() {
}

void RemWordToolWdg::slotBtnOpenFileTransform(void) {
    cleanWdg();
    mspWordTranform->vsMergeWord.clear();
    mspWordTranform->vsEnglishWord.clear();
    mspWordTranform->vsChineseWord.clear();
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
    if (!mspWordTranform->readFileDatafstream(msTransformFilePath)) {
        QMessageBox::warning(this, "警告", "警告！中英文单词数量不相等");
        return;
    }
    showEnglishWordTxt();
}

void RemWordToolWdg::slotBtnTransform(void) {
    cleanWdg();
    mspWordTranform->vsMergeWord.clear();
    QString transformOutputFilePath = QFileDialog::getSaveFileName(
        this, "Selecting Source Output File", QString::fromStdString(msOpenPath), "Markdown (*.md);; Other(*)");
    if (transformOutputFilePath.isEmpty()) {
        LOG_WRN("No transform file path");
        return;
    }

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
    if (!mspWordTranform->mergeChineseAndEnglishWord()) {
        return;
    }
    ui->progressBar->setValue(mProgressBarMax * 0.6);
    if (!mspWordTranform->writeMergeWord(msTransformOutputFilePath)) {
        return;
    }
    ui->progressBar->setValue(mProgressBarMax * 1);
    showMergeWord();
}

void RemWordToolWdg::slotBtnOpenFileImport(void) {
    cleanWdg();
    QString importFilePath = QFileDialog::getOpenFileName(
        this, "Selecting Import file", QString::fromStdString(msOpenPath), "Markdown (*.md);; Other(*)");
    if (importFilePath.isEmpty()) {
        LOG_WRN("No import file path");
        return;
    }
    ui->leImportPath->setText(importFilePath);
    msImportFilPath = importFilePath.toStdString();
    LOG_INF("Read import file path: {}", msImportFilPath);

    if (!mspWordImport->readImportFileDatafgets(msImportFilPath)) {
        return;
    }
    showPreImportTxt();
}

void RemWordToolWdg::slotBtnImport(void) {
    cleanWdg();
    // TODO 考虑这个结构的内存释放
    // TODO 考虑直接点击Import
    mspWordImport->rootWordTree = std::make_shared<WordTreeNode>("root", false);
    mspWordImport->vsWordInfo.clear();
    // QString importOutputFilePath = QFileDialog::getSaveFileName(
    //     this, "Selecting Import file", QString::fromStdString(msOpenPath), "Markdown (*.md);; Other(*)");
    // if (importOutputFilePath.isEmpty()) {
    //     LOG_WRN("No import file path");
    //     return;
    // }

    msImportOutputFilePath = msImportFilPath;
    LOG_INF("Import file output path: {}", msImportOutputFilePath);

    if (!mspWordImport->parseInfoFromPreImport()) {
        return;
    }
    if (!mspWordImport->initOutputFilename(msImportOutputFilePath)) {
        return;
    }
    if (!mspWordImport->processWordTree2Vector()) {
        return;
    }
    if (!mspWordImport->writeWordMd()) {
        return;
    }
    if (!mspWordImport->saveWord2Sqlite()) {
        return;
    }
}

bool RemWordToolWdg::showEnglishWordTxt(void) {
    // TODO
    // size_t wordLen = vsChineseWord.size();
    // for (size_t i = 0; i < wordLen; i++) {
    //    ui->textBrowser->append(QString::fromLocal8Bit(vsEnglishWord[i].c_str()));
    //    ui->textBrowser->append(QString::fromLocal8Bit(vsChineseWord[i].c_str()));
    //}
    return true;
}

bool RemWordToolWdg::showMergeWord(void) {
    ui->textBrowser->append(QString("# Title"));
    ui->textBrowser->append(QString("## Sub Title"));
    // TODO
    // for (std::string& str : vsMergeWord) {
    //    // 只有这种方式string转QString中文不会乱码
    //    const QString qstringMergeWord = QString::fromLocal8Bit(str.c_str());
    //    ui->textBrowser->append(qstringMergeWord);
    //}
    return true;
}

bool RemWordToolWdg::cleanWdg(void) {
    QString browserText = ui->textBrowser->toPlainText();
    if (browserText != QString("")) {
        ui->textBrowser->clear();
    }
    return true;
}

bool RemWordToolWdg::showPreImportTxt(void) {
    // TODO
    // int wordLen = vsPreImportWord.size();
    // for (int i = 0; i < wordLen; i++) {
    //    ui->textBrowser->append(QString::fromLocal8Bit(vsPreImportWord[i].c_str()));
    //}
    return true;
}
