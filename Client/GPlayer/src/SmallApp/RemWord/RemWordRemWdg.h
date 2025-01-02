#pragma once

#include <QWidget>
#include "WordSql.h"
#include "WrtSglWdg.h"
#include "global.h"
#include "ui_RemWordRemWdg.h"

class RemWordRemWdg : public QWidget {
    Q_OBJECT

public:
    RemWordRemWdg(QWidget *parent = nullptr);
    ~RemWordRemWdg();

public:
    void slotBtnTest(void);
    void slotBtnBatchConfirm(void);
    void slotDateChange(void);
    void slotBtnClean(void);

private:
    void initUI(void);
    void initWordList(void);
    void RemWordRemWdg::processMemoryResult(std::vector<int32> errWord);

    Ui::RemWordRemWdg *ui;
    QGridLayout *pGridLayout;
    std::shared_ptr<WordSql> mspWordSql;
    std::vector<WordSqlInfo> mvWordList;
    std::vector<bool> mvbWordListAns;
    std::vector<uint8> mvEbbinghausForgettingCurve;
    std::vector<std::shared_ptr<WrtSglWdg>> mvspWrtSglWdg;
};
