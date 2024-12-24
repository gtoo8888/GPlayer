#include "ClockSetWdg.h"
#include "ui_ClockSetWdg.h"

ClockSetWdg::ClockSetWdg(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ClockSetWdg) {
    ui->setupUi(this);
}

ClockSetWdg::~ClockSetWdg() {
    delete ui;
}
