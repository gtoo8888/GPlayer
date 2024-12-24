#pragma once

#include <QWidget>
#include "ui_RemWordListWdg.h"

class RemWordListWdg : public QWidget {
    Q_OBJECT

public:
    RemWordListWdg(QWidget *parent = nullptr);
    ~RemWordListWdg();

private:
    Ui::RemWordListWdg *ui;
};
