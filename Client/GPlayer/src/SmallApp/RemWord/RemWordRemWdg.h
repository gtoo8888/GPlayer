#pragma once

#include <QWidget>
#include "ui_RemWordRemWdg.h"
#include "WrtSglWdg.h"

class RemWordRemWdg : public QWidget {
    Q_OBJECT

public:
    RemWordRemWdg(QWidget *parent = nullptr);
    ~RemWordRemWdg();

public:
    void slotBtnTest(void);

private:
    Ui::RemWordRemWdg *ui;
    QVBoxLayout *vlayout;
};
