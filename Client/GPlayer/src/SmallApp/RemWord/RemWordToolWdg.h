#pragma once

#include <QWidget>
#include "WordSql.h"
#include "global.h"
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
class WordTranform;
class WordImport;

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
    bool cleanWdg(void);

    int64_t mProgressBarMin;
    int64_t mProgressBarMax;
    std::string msOpenPath;
    std::string msTransformFilePath;
    std::string msTransformOutputFilePath;
    std::string msImportFilPath;
    std::string msImportOutputFilePath;

    // Tranform
    bool showEnglishWordTxt(void);
    bool showMergeWord(void);
    // Import
    bool showPreImportTxt(void);


    std::shared_ptr<WordTranform> mspWordTranform;
    std::shared_ptr<WordImport> mspWordImport;
};

class WordTranform {
public:
    WordTranform();
    ~WordTranform() = default;
    bool readFileDatafstream(std::string filePath);
    bool readFileDatafgets(std::string filePath);
    bool readFileDataQFile(std::string filePath);
    bool mergeChineseAndEnglishWord(void);
    bool writeMergeWord(std::string filePath);

    int32 mEnglishTotalLen;
    std::vector<std::string> vsEnglishWord;
    std::vector<std::string> vsChineseWord;
    std::vector<std::string> vsMergeWord;
};

class WordImport {
public:
    WordImport();
    ~WordImport() = default;

    bool readImportFileDatafgets(std::string filePath);
    bool initOutputFilename(std::string filePath);
    bool parseInfoFromPreImport(void);
    bool processWordTree2Vector(void);
    bool writeWordMd(void);
    bool saveWord2Sqlite(void);
    bool testWord2Sqlite(void);
    bool writeLanguagueMd(std::string path, LanguageType type);
    std::string wordInfo2String(WordInfo wordInfo, LanguageType type);

    int32 mEnglishTotalLen;
    std::vector<std::string> vsPreImportWord;
    std::string oFnChinese;
    std::string oFnEnglish;
    std::string oFnCnAdnEn;
    std::shared_ptr<WordTreeNode> rootWordTree;  // 类似树的结构保存章节信息
    std::vector<std::string> vsOChineseWord;
    std::vector<std::string> vsOEnglishWord;
    std::vector<std::string> vsOChAndEnWord;
    std::vector<WordInfo> vsWordInfo;

    std::shared_ptr<WordSql> mspWordSql;
};