#include "ClockWdg.h"
#include <QDebug>
#include "StopwatchWdg.h"
#include "ui_ClockWdg.h"

ClockWdg::ClockWdg(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::ClockWdg) {
    ui->setupUi(this);
}

ClockWdg::~ClockWdg() {
    delete ui;
}

void ClockWdg::on_btnStopwatch_clicked() {
    qDebug() << "stopwatch";

    StopwatchWdg* stopwatchWdg = new StopwatchWdg();
    ui->widget_2->hide();

    stopwatchWdg->show();
}
