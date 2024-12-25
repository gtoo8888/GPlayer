#pragma once

#include <QWidget>
#include "ui_WrtSglWdg.h"

class WrtSglWdg : public QWidget {
    Q_OBJECT

public:
    WrtSglWdg(QWidget* parent = nullptr);
    ~WrtSglWdg();

private:
    Ui::WrtSglWdg* ui;

};
