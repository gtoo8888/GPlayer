#pragma once

#include "global.h"
#include "ui_VideoCtrl.h"

class VideoCtrl : public QWidget {
    Q_OBJECT

public:
    VideoCtrl(QWidget* parent = nullptr);
    ~VideoCtrl();

private:
    Ui::VideoCtrl* ui;
};
