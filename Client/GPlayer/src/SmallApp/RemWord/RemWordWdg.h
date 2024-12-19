#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include "RemWordToolWdg.h"
#include "ui_RemWordWdg.h"

class RemWordWdg : public QWidget {
    Q_OBJECT

public:
    RemWordWdg(QWidget* parent = nullptr);
    ~RemWordWdg();

public slots:
    void slotOpenToolWdg(void);

private:
    Ui::RemWordWdg* ui;
    QVBoxLayout* vlayout;
};
