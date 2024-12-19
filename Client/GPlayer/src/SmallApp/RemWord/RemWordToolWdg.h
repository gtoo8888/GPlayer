#pragma once

#include <QWidget>
#include <fstream>
#include <string>
#include <vector>
#include "GtooLogger.h"
#include "WordSql.h"
#include "ui_RemWordToolWdg.h"

class WordInfo {
public:
    int idx;
    std::string english;
    std::string chinese;

    WordInfo() = default;
    WordInfo(int i, const std::string& en, const std::string& ch)
        : idx(i),
          english(en),
          chinese(ch) {
    }
};

// 主标题和子标题，是level<2,使用title，subTitle保存
// 具体的单词，是level=2,使用vWord保存
class WordTreeNode {
public:
    std::string title;
    bool isLeaf;
    std::vector<std::shared_ptr<WordTreeNode>> subTitle;
    std::vector<WordInfo> words;

    WordTreeNode(const std::string& ti, bool leaf = false)
        : title(ti),
          isLeaf(leaf) {
    }

    void addSubTitle(std::shared_ptr<WordTreeNode> spWtn) {
        if (!isLeaf) {
            subTitle.push_back(spWtn);
        }
    }

    void addWord(const WordInfo& word) {
        if (isLeaf) {
            words.push_back(word);
        }
    }
};

enum LanguageType {
    Chinese,
    English,
    ChAndEn
};

class WordSql;


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
    Ui::RemWordToolWdg* ui;
    // TODO 变量统一做初始化
    // 保存的txt需要保存格式为ANSI
    // Tranform
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

    int mEnglishTotalLen;
    int64_t mProgressBarMin;
    int64_t mProgressBarMax;
    std::string msOpenPath;
    std::string msTransformFilePath;
    std::string msTransformOutputFilePath;
    std::string msImportFilPath;
    std::string msImportOutputFilePath;
    std::vector<std::string> vsEnglishWord;
    std::vector<std::string> vsChineseWord;
    std::vector<std::string> vsMergeWord;

    // Import
    bool readImportFileDatafgets(std::string filePath);
    bool showPreImportTxt(void);
    bool initOutputFilename(std::string filePath);
    bool parseInfoFromPreImport(void);
    bool splitStringAtDelimiter(const std::string& str, char delimiter, std::string& left, std::string& right);
    bool processWordTree2Vector(void);
    bool writeWordMd(void);
    bool writeLanguagueMd(std::string path, LanguageType type);
    bool saveWord2Sqlite(void);

    std::string getFilename(const std::string& path);
    std::string getFilenameWithoutExe(const std::string& path);
    std::string getFileSuffix(const std::string& path);
    std::string getAbsolutePath(const std::string& path);
    std::string wordInfo2String(WordInfo wordInfo, LanguageType type);

    std::string oFnChinese;
    std::string oFnEnglish;
    std::string oFnCnAdnEn;
    std::vector<std::string> vsPreImportWord;
    std::shared_ptr<WordTreeNode> rootWordTree;  // 类似树的结构保存章节信息
    std::vector<std::string> vsOChineseWord;
    std::vector<std::string> vsOEnglishWord;
    std::vector<std::string> vsOChAndEnWord;

    std::shared_ptr<WordSql> mspWordSql;
};
