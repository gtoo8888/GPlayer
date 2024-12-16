#pragma once

#include <QWidget>
#include <fstream>
#include <string>
#include <vector>
#include "GtooLogger.h"
#include "ui_RemWordToolWdg.h"

class OriginWord {};

class RemWordToolWdg : public QWidget {
    Q_OBJECT

public:
    RemWordToolWdg(QWidget* parent = nullptr);
    ~RemWordToolWdg();

public:
    void slotBtnOpenFileTransform(void);
    void slotBtnTransform(void);
    void slotBtnOpenFileImport(void);
    void slotBtnImport(void);

private:
    // 保存的txt需要保存格式为ANSI
    bool readFileDatafstream(std::string filePath);
    bool readFileDatafgets(std::string filePath);
    bool readFileDataQFile(std::string filePath);
    bool isLetterC(char letter);
    bool isLetterCplusplus(char letter);
    bool mergeChineseAndEnglishWord(void);
    bool writeMergeWord(std::string filePath);
    bool showEnglishWordTxt(void);
    bool showMergeWord(void);
    bool cleanWdg(void);

    // TODO 变量统一做初始化
    Ui::RemWordToolWdg* ui;
    int mEnglishTotalLen;
    int64_t mProgressBarMin;
    int64_t mProgressBarMax;
    std::string msTransformFilePath;
    std::string msTransformOutputFilePath;
    std::string msImportFilPath;
    std::string msImportOutputFilePath;
    std::vector<std::string> vsEnglishWord;
    std::vector<std::string> vsChineseWord;
    std::vector<std::string> vsMergeWord;
};
