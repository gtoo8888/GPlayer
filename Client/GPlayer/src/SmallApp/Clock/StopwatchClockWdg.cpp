#include "StopwatchClockWdg.h"
#include <QDebug>
#include <QEvent>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include "ui_StopwatchClockWdg.h"

StopwatchClockWdg::StopwatchClockWdg(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::StopwatchClockWdg),
      clockNowStatus(stop),
      nameModifyStatus(lock),
      updateTimer(new QTimer),
      clockIndex(-1),
      nowClockCount(0),
      initTimeStr(QString("00:00:00.000")),
      stopwatchClockName(QString("Stopwatch")) {
    ui->setupUi(this);

    initUi();
    initConnect();
}

void StopwatchClockWdg::initUi() {
    ui->labShowTime->setText(initTimeStr);
    cleanTableWdg();
    ui->tableWdgTimeCount->verticalHeader()->setVisible(false);
    ui->tableWdgTimeCount->setSelectionBehavior(QAbstractItemView::SelectRows);  // 可以选中一行
    ui->tableWdgTimeCount->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 下面两行，自动调整列宽
    ui->tableWdgTimeCount->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWdgTimeCount->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    ui->leClockName->setEchoMode(QLineEdit::Normal);
    ui->leClockName->setEnabled(false);  // 不可修改
    ui->leClockName->setText(stopwatchClockName);

    ui->btnTimeCount->setDisabled(true);
    ui->widgetAll->installEventFilter(this);
    //    ui->labIcon->installEventFilter(this);
}

void StopwatchClockWdg::initConnect() {
    connect(ui->btnStart, &QPushButton::clicked, this, &StopwatchClockWdg::slotBtnStart);
    connect(ui->btnReset, &QPushButton::clicked, this, &StopwatchClockWdg::slotBtnReset);
    connect(ui->btnDownload, &QPushButton::clicked, this, &StopwatchClockWdg::slotBtnDownload);
    connect(ui->btnTimeCount, &QPushButton::clicked, this, &StopwatchClockWdg::slotBtnTimeCount);
    connect(ui->btnNameModify, &QPushButton::clicked, this, &StopwatchClockWdg::slotBtnNameModify);
    connect(ui->btnClockClose, &QPushButton::clicked, this, &StopwatchClockWdg::slotBtnClockClse);
    connect(this->updateTimer, &QTimer::timeout, this, &StopwatchClockWdg::slotUpdateDisplay);
}

void StopwatchClockWdg::cleanTableWdg() {
    ui->tableWdgTimeCount->clear();
    while (ui->tableWdgTimeCount->rowCount() > 0) {
        ui->tableWdgTimeCount->removeRow(0);
    }
    ui->tableWdgTimeCount->setHorizontalHeaderLabels(QStringList() << "序号" << "间隔" << "总计");
}

StopwatchClockWdg::~StopwatchClockWdg() {
    delete ui;
}

void StopwatchClockWdg::setClockIndex(uint8_t idx) {
    this->clockIndex = idx;
    ui->labClockNum->setText(QString("秒表%1").arg(clockIndex));
}

// 1.点击开始后获取到当前的时间并且赋值给baseTime
// 2.启动定时器,间隔1s
// 3.槽函数中再次获取当前的时间currTime
// 4.计算两个时间的差值t
// 5.声明一个showTime对象给他加上t的差值
// 6.格式化后设置显示
void StopwatchClockWdg::slotUpdateDisplay() {
    QTime showTime(0, 0, 0, 0);
    timeStr = showTime.addMSecs(startTime.msecsTo(QTime::currentTime())).toString("hh:mm:ss.zzz");
    ui->labShowTime->setText(timeStr);
}

bool StopwatchClockWdg::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->widgetAll) {
        if (QEvent::MouseButtonRelease == event->type()) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                ui->widgetAll->setStyleSheet("#widgetAll{border:3px solid blue}");
                LOG_INF("widgetAll LeftButton");
                return true;
            } else if (mouseEvent->button() == Qt::RightButton) {
                ui->widgetAll->setStyleSheet("");
                LOG_INF("widgetAll RightDbButton");
                return true;
            }
        }
    }
    return false;
}

void StopwatchClockWdg::slotBtnStart() {
    static QTime pauseTime;  // 通过临时变量控制暂停
    if (clockNowStatus == ClockStatus::stop) {
        ui->btnTimeCount->setDisabled(false);
        clockNowStatus = ClockStatus::run;
        this->startTime = this->startTime.currentTime();
        this->updateTimer->start();
        ui->btnStart->setText("暂停");
    } else if (clockNowStatus == ClockStatus::pause) {
        ui->btnTimeCount->setDisabled(false);
        clockNowStatus = ClockStatus::run;
        QTime cut = QTime::currentTime();
        int t = pauseTime.msecsTo(cut);
        this->startTime = this->startTime.addMSecs(t);
        this->updateTimer->start();
        ui->btnStart->setText("暂停");
    } else if (clockNowStatus == ClockStatus::run) {
        ui->btnTimeCount->setDisabled(true);
        pauseTime = QTime::currentTime();
        clockNowStatus = ClockStatus::pause;
        ui->btnStart->setText("开始");
        this->updateTimer->stop();
    }
}

void StopwatchClockWdg::slotBtnReset() {
    clockNowStatus = ClockStatus::stop;
    ui->btnStart->setText("开始");
    this->updateTimer->stop();
    ui->labShowTime->setText(initTimeStr);
    nowClockCount = 0;
    ui->btnTimeCount->setDisabled(true);
    cleanTableWdg();
}

void StopwatchClockWdg::slotBtnDownload() {
    uint8_t nowCout = ui->tableWdgTimeCount->rowCount();
    if (nowCout == 0) {
        QMessageBox::warning(this, "警告", "警告！保存内容不能为空!");
        return;
    }

    QDateTime dateTime = QDateTime::currentDateTime();        // 获取系统当前的时间
    QString dateTimeStr = dateTime.toString("yyyyMMddhhmm");  // 格式化时间
    QString saveFileName = QString("%1_%2").arg(stopwatchClockName).arg(dateTimeStr);
    QString filename = QFileDialog::getSaveFileName(this, "保存秒表记录", saveFileName, "(*.csv)");

    if (filename.isEmpty()) return;

    QString outStr = QString("idx,interval time,total time\n");
    for (int i = 0; i < nowCout; i++) {
        QString outStrLine = QString("%1,%2,%3\n")
                                 .arg(vTimeCount[i].index, 2, 10, QLatin1Char('0'))
                                 .arg(vTimeCount[i].intervalTimeStr)
                                 .arg(vTimeCount[i].totalTimeStr);
        outStr += outStrLine;
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        qDebug() << "Cannot open file for writing:" << file.errorString();
        return;
    }
    QTextStream stream(&file);
    stream << outStr;
    if (file.error() != QFile::NoError) {
        qDebug() << "An error occurred while writing to the file:" << file.errorString();
        return;
    }
    if (file.error() != QFile::NoError) {
        qDebug() << "An error occurred while closing the file:" << file.errorString();
        return;
    }
    QMessageBox box(QMessageBox::Information, QString("提示"), QString("保存成功"));
    box.exec();
}

void StopwatchClockWdg::slotBtnTimeCount() {
    QString totalTimeStr = ui->labShowTime->text();
    QString intervalTimeStr = totalTimeStr;
    QDateTime endDateTime = QDateTime::fromString(totalTimeStr, "hh:mm:ss.zzz");
    QDateTime startDateTime;
    qint64 msecs = 0;
    if (nowClockCount == 0) {
        startDateTime = endDateTime;
    } else if (nowClockCount >= 1) {
        QString startDateTimeStr = vTimeCount[nowClockCount - 1].totalTimeStr;
        startDateTime = QDateTime::fromString(startDateTimeStr, "hh:mm:ss.zzz");
        msecs = startDateTime.msecsTo(endDateTime);
        QTime showTime(0, 0, 0, 0);
        intervalTimeStr = showTime.addMSecs(msecs).toString("hh:mm:ss.zzz");
    }

    LOG_INF("idx:{:d},interval time:{},total time:{}", nowClockCount, intervalTimeStr.toStdString(),
            totalTimeStr.toStdString());
    //    qDebug()<< QString("idx:%1,interval time:%2,total time:%3").arg(nowClockCount)
    //               .arg(intervalTimeStr).arg(totalTimeStr);

    nowClockCount += 1;
    // TODO VS2017不支持
    // TimeCountInfo nowInfo{
    //     .index = nowClockCount,
    //     .intervalTimeStr = intervalTimeStr,
    //     .totalTimeStr = totalTimeStr,
    //     .slowIdx = 255,
    //     .fastIdx = 255,
    // };
    TimeCountInfo nowInfo;
    nowInfo.index = nowClockCount;
    nowInfo.intervalTimeStr = intervalTimeStr;
    nowInfo.totalTimeStr = totalTimeStr;
    nowInfo.slowIdx = 255;
    nowInfo.fastIdx = 255;

    vTimeCount.push_back(nowInfo);

    uint8_t tableWdgIdx = ui->tableWdgTimeCount->rowCount();
    ui->tableWdgTimeCount->setRowCount(tableWdgIdx + 1);  // 一定要设置行数以后才能显示
    ui->tableWdgTimeCount->setItem(tableWdgIdx, 0, new QTableWidgetItem(QString::number(nowInfo.index)));
    ui->tableWdgTimeCount->setItem(tableWdgIdx, 1, new QTableWidgetItem(nowInfo.intervalTimeStr));
    ui->tableWdgTimeCount->setItem(tableWdgIdx, 2, new QTableWidgetItem(nowInfo.totalTimeStr));
}

void StopwatchClockWdg::slotBtnNameModify() {
    if (nameModifyStatus == NameModifyStatus::lock) {
        nameModifyStatus = NameModifyStatus::modify;
        ui->leClockName->setEnabled(true);
        ui->btnNameModify->setText("确定");
    } else if (nameModifyStatus == NameModifyStatus::modify) {
        nameModifyStatus = NameModifyStatus::lock;
        ui->leClockName->setEnabled(false);
        stopwatchClockName = ui->leClockName->text();
        ui->btnNameModify->setText("修改");
        qDebug() << stopwatchClockName;
    }
}

void StopwatchClockWdg::slotBtnClockClse() {
    this->close();
    emit sigClockCloseWnd(clockIndex);
}
