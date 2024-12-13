#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include "ui_RemWordWdg.h"
#include "RemWordToolWdg.h"

class RemWordWdg : public QWidget
{
    Q_OBJECT

public:
    RemWordWdg(QWidget *parent = nullptr);
    ~RemWordWdg();

public slots:
    void slotOpenToolWdg(void);

private:
    Ui::RemWordWdg* ui;
    QVBoxLayout* vlayout;
};
