#include "StopwatchSetWdg.h"
#include "ui_StopwatchSetWdg.h"

StopwatchSetWdg* StopwatchSetWdg::instanceSetWdg = nullptr;

StopwatchSetWdg::StopwatchSetWdg(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::StopwatchSetWdg) {
    ui->setupUi(this);

    QString configPath =
        QString("E:\\Desktop\\languguetest\\Cplusplustest\\4-QtTest\\1-Project\\2-SmallApp\\Clock\\ClockCfg.json");
    clockConfig = new ClockConfig(configPath);

    initUi();
}

StopwatchSetWdg::~StopwatchSetWdg() {
    delete ui;
}

void StopwatchSetWdg::initUi() {
    this->resize(600, 400);
    this->setMinimumSize(QSize(600, 400));
    this->setMaximumSize(QSize(600, 400));
    this->setWindowIcon(QIcon(":/Stopwatch/pic/Set.svg"));

    ui->tabWidget->setTabText(0, QString("设置"));
    ui->tabWidget->setTabText(1, QString("快捷键"));
    ui->tabWidget->setTabText(2, QString("关于"));
    ui->tabWidget->setCurrentIndex(0);

    // 设置界面
    ui->leStopwatchCount->setText(QString::number(clockConfig->settingConfig->StopwatchCount,10));
    ui->cobDisplayUnit->addItems(QStringList() << "秒" << "毫秒"); // TOFIX
    ui->cobDisplayUnit->setCurrentIndex(clockConfig->settingConfig->displayUnit);
    ui->chbDrag->setChecked(clockConfig->settingConfig->bDrag);
    ui->chbSlowFastDisplay->setChecked(clockConfig->settingConfig->bSlowFastDisplay);
    ui->chbAutoClean->setChecked(clockConfig->settingConfig->bAUtoClean);
    ui->leAutoCleanTime->setText(QString::number(clockConfig->settingConfig->autoCleanTime,10));
    ui->cobLanguague->addItems(QStringList() << "中文" << "英语"); // TOFIX
    ui->cobLanguague->setCurrentIndex(clockConfig->settingConfig->languague);

    // 快捷键界面
    ui->leKeyStop->setText(QString("Space"));
    ui->leKeyStop->setEnabled(false);
    ui->btnKeyStop->setText("修改");

    ui->leKeyCout->setText(QString("Ctrl+D"));
    ui->leKeyClose->setText(QString("Esc"));
    ui->leKeyNew->setText(QString("N"));
    ui->btnKeyCount->setText("修改");
    ui->btnKeyClose->setText("修改");
    ui->btnKeyNew->setText("修改");

    // 关于界面
    ui->labAppName->setText(QString::fromStdString(clockConfig->aboutConfig->appName));
    ui->labAppVersion->setText(QString("版本号：v%1").arg(QString::fromStdString(clockConfig->aboutConfig->appVersion)));
}
