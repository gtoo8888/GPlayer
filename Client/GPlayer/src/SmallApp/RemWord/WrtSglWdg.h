#pragma once

#include <QWidget>
#include "WordSql.h" // TODO 评估引入整个类是否合理？
#include "global.h"
#include "ui_WrtSglWdg.h"

// WordRememberTranslateSingalWdg WrtSglWdg
class WrtSglWdg : public QWidget {
    Q_OBJECT

public:
    WrtSglWdg(QWidget* parent = nullptr);
    WrtSglWdg(WordSqlInfo wordSqlInfo, QWidget* parent = nullptr);
    ~WrtSglWdg();

public:
    bool slotBtnConfirm(void);
    bool checkMemeryEnglish(std::string word);
    int32 getWordIdx(void);

private:
    Ui::WrtSglWdg* ui;

    WordSqlInfo mWordSqlInfo;
    int32 mWordIdx;
    std::string msChinesWord;
    std::string msEnglishWord;
};
