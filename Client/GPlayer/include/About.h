#pragma once

#include <QObject>
#include "ui_About.h"

class About  : public QWidget
{
    Q_OBJECT

public:
    About(QWidget *parent = nullptr); // 一定需要初始化，不然会报错
    ~About();

    void closeCurrentWindow();

private:
    Ui::About* ui;
};
