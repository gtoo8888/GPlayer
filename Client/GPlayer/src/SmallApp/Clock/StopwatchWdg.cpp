#include "StopwatchWdg.h"
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include "StopwatchSetWdg.h"
#include "ui_StopwatchWdg.h"

StopwatchWdg::StopwatchWdg(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::StopwatchWdg),
      maxClockNum(6),
      updateTimer(new QTimer) {
    ui->setupUi(this);
    vlayout = new QVBoxLayout(this);
    ui->clockShowWdg->setLayout(vlayout);
    ui->labIcon->installEventFilter(this);
    setWindowIcon(QIcon(":/Stopwatch/pic/Stopwatch.svg"));

    // 判断系统是否支持托盘图标显示
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        return;
    }

    // 实例化托盘图标控件
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/Stopwatch/pic/Stopwatch.svg"));  // 设置托盘图标显示
    trayIcon->setToolTip("秒表");                               // 显示提示信息
    trayIcon->showMessage("托盘", "托盘管理", QSystemTrayIcon::Information, 10000); // FIXME 消息不显示
    trayIcon->show();                                           // 在任务栏显示图标

    connect(ui->btnNewClock, &QPushButton::clicked, this, &StopwatchWdg::slotBtnNewClock);
    connect(ui->btnCleanClock, &QPushButton::clicked, this, &StopwatchWdg::slotBtnCleanClock);
    connect(this->updateTimer, &QTimer::timeout, this, &StopwatchWdg::slotupdateNowTime);

    updateTimer->start();

    // TOTEXT
    StopwatchSetWdg* stopwatchSetWdg = StopwatchSetWdg::getInstance();
    stopwatchSetWdg->show();

    LOG_INF("  ");
    LOG_INF("init StopwatchWdg");
}

StopwatchWdg::~StopwatchWdg() {
    delete ui;
}

void StopwatchWdg::slotBtnNewClock(void) {
    int nowClockNum = clockWdgVec.size();
    if (nowClockNum < maxClockNum) {
        StopwatchClockWdg* stopwatchClockWdg = new StopwatchClockWdg(ui->clockShowWdg);
        clockWdgVec.push_back(stopwatchClockWdg);
        vlayout->addWidget(stopwatchClockWdg);
        stopwatchClockWdg->setClockIndex(clockWdgVec.size());
        connect(stopwatchClockWdg, &StopwatchClockWdg::sigClockCloseWnd, this, &StopwatchWdg::slotClockClose);
        stopwatchClockWdg->show();
    } else {
        QMessageBox box(QMessageBox::Warning, QString("警告"), QString("当前秒表达到最大%1个").arg(nowClockNum));
        box.exec();
    }
}

void StopwatchWdg::slotBtnCleanClock() {
    for (auto it = clockWdgVec.begin(); it != clockWdgVec.end(); it++) {
        StopwatchClockWdg* deleteClock = *it;
        delete deleteClock;
    }
    clockWdgVec.clear();
}

void StopwatchWdg::slotClockClose(int clockIndex) {
    StopwatchClockWdg* nowClockClose = clockWdgVec[clockIndex - 1];
    delete nowClockClose;
    clockWdgVec.erase(clockWdgVec.begin() + clockIndex - 1);

    for (int idx = 0; idx < clockWdgVec.size(); idx++) {
        clockWdgVec[idx]->setClockIndex(idx + 1);
    }
}

void StopwatchWdg::slotupdateNowTime(void) {
    ui->labShowTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

bool StopwatchWdg::eventFilter(QObject* watched, QEvent* event) {
    if (watched == ui->labIcon) {
        if (QEvent::MouseButtonDblClick == event->type()) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                LOG_INF("LeftDbButton");
                slotEventLabSettingClock();
                return true;
            } else if (mouseEvent->button() == Qt::RightButton) {
                LOG_INF("RightDbButton");
                return true;
            }
        }
    }
    return false;
}

void StopwatchWdg::keyPressEvent(QKeyEvent* event) {
    int keyType = event->key();
    switch (keyType) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            LOG_INF("Key_Enter");
            break;
        case Qt::Key::Key_Escape:
            LOG_INF("Key_Escape");
            break;
        case Qt::Key::Key_Space:
            LOG_INF("Key_Space");
            break;
    }
}

void StopwatchWdg::slotEventLabSettingClock() {
    StopwatchSetWdg* stopwatchSetWdg = StopwatchSetWdg::getInstance();
    stopwatchSetWdg->show();
}
