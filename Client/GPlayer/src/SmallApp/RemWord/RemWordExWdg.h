#pragma once

#include <QWidget>
#include "ui_RemWordExWdg.h"

class RemWordExWdg : public QWidget {
    Q_OBJECT

public:
    RemWordExWdg(QWidget *parent = nullptr);
    ~RemWordExWdg();

private:
    Ui::RemWordExWdg *ui;
};
