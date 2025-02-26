#include "VideoCtrl.h"

VideoCtrlWdg::VideoCtrlWdg(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::VideoCtrlWdg) {
    ui->setupUi(this);

    int32 sliderDefaultValue = 50;
    ui->sldBrightness->setValue(sliderDefaultValue);
    ui->sldContrast->setValue(sliderDefaultValue);
    ui->sldSaturation->setValue(sliderDefaultValue);
    ui->sldColor->setValue(sliderDefaultValue);

    connect(ui->sldBrightness, &QSlider::valueChanged, this, &VideoCtrlWdg::sigSldBrightnessChanged);
    connect(ui->sldContrast, &QSlider::valueChanged, this, &VideoCtrlWdg::sigSldContrastChanged);
    connect(ui->sldSaturation, &QSlider::valueChanged, this, &VideoCtrlWdg::sigSldSaturationChanged);
    connect(ui->sldColor, &QSlider::valueChanged, this, &VideoCtrlWdg::sigSldColorChanged);
}

VideoCtrlWdg::~VideoCtrlWdg() {
}
