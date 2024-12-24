#ifndef STOPWATCHWDG_H
#define STOPWATCHWDG_H

#include <QHBoxLayout>
#include <QString>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>
#include <QSystemTrayIcon>
#include "GtooLogger.h"
#include "StopwatchClockWdg.h"

namespace Ui {
class StopwatchWdg;
}

class StopwatchWdg : public QWidget {
    Q_OBJECT

public:
    explicit StopwatchWdg(QWidget *parent = nullptr);
    ~StopwatchWdg();

public slots:
    void slotBtnNewClock(void);
    void slotBtnCleanClock(void);
    void slotClockClose(int clockIndex);
    void slotupdateNowTime(void);

    bool eventFilter(QObject *watched, QEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void slotEventLabSettingClock(void);  // 事件过滤器伪装的槽实现
    Ui::StopwatchWdg *ui;

    QVBoxLayout *vlayout;
    QVector<StopwatchClockWdg *> clockWdgVec;
    uint8_t maxClockNum;
    QTimer *updateTimer;
    QSystemTrayIcon* trayIcon;
};

#endif  // STOPWATCHWDG_H
