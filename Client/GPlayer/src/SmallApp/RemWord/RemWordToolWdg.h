#pragma once

#include <QWidget>
#include "ui_RemWordToolWdg.h"

class RemWordToolWdg : public QWidget
{
    Q_OBJECT

public:
    RemWordToolWdg(QWidget *parent = nullptr);
    ~RemWordToolWdg();

private:
    Ui::RemWordToolWdgClass ui;
};
