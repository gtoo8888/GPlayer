#include "StopwatchSetShortKey.h"
#include "ui_StopwatchSetShortKey.h"

StopwatchSetShortKey::StopwatchSetShortKey(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StopwatchSetShortKey)
{
    ui->setupUi(this);
}

StopwatchSetShortKey::~StopwatchSetShortKey()
{
    delete ui;
}
