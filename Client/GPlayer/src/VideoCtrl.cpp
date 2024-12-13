#include "VideoCtrl.h"

VideoCtrl::VideoCtrl(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::VideoCtrl) {
    ui->setupUi(this);
}

VideoCtrl::~VideoCtrl() {
}
