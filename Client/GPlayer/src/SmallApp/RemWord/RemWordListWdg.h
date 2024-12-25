#pragma once

#include <QWidget>
#include "WordSql.h"
#include "global.h"
#include "ui_RemWordListWdg.h"

class RemWordListWdg : public QWidget {
    Q_OBJECT

public:
    RemWordListWdg(QWidget *parent = nullptr);
    ~RemWordListWdg();

    void initUI(void);
    void cleanTableWdg(void);
    void updateTableWdg(void);
    void initWordList(void);
    QString unix2StrTimeQString(int64 time);
    std::string unix2StrTimeChrono(int64 time);

private:
    Ui::RemWordListWdg *ui;
    std::shared_ptr<WordSql> mspWordSql;
    std::vector<WordSqlInfo> mvWordList;
};
