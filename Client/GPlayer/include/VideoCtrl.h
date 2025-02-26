#pragma once

#include "global.h"
#include "ui_VideoCtrl.h"

class VideoCtrlWdg : public QWidget {
    Q_OBJECT

public:
    VideoCtrlWdg(QWidget* parent = nullptr);
    ~VideoCtrlWdg();

signals:
    void sigSldBrightnessChanged(int value);
    void sigSldContrastChanged(int value);
    void sigSldSaturationChanged(int value);
    void sigSldColorChanged(int value);

private:
    Ui::VideoCtrlWdg* ui;
};
