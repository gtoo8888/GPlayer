#pragma once

#include <QWidget>
#include "ui_RemWordRemWdg.h"

class RemWordRemWdg : public QWidget {
    Q_OBJECT

public:
    RemWordRemWdg(QWidget *parent = nullptr);
    ~RemWordRemWdg();

private:
    Ui::RemWordRemWdg *ui;
};
