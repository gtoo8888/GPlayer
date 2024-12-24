#ifndef STOPWATCHCLOCKWDG_H
#define STOPWATCHCLOCKWDG_H

#include <QTime>
#include <QTimer>
#include <QVector>
#include <QWidget>
#include "GtooLogger.h"

namespace Ui {
class StopwatchClockWdg;
}

typedef struct {
    uint8_t index;
    QString intervalTimeStr;
    QString totalTimeStr;
    uint8_t slowIdx;
    uint8_t fastIdx;
} TimeCountInfo;

class StopwatchClockWdg : public QWidget {
    Q_OBJECT
public:
    enum ClockStatus {
        stop = 0,
        pause,
        run
    };

    enum NameModifyStatus {
        modify = 0,
        lock,
    };

public:
    explicit StopwatchClockWdg(QWidget *parent = nullptr);
    ~StopwatchClockWdg();
    void setClockIndex(uint8_t idx);

signals:
    void sigClockCloseWnd(int clockIndex);

public slots:
    void slotBtnStart(void);
    void slotBtnReset(void);
    void slotBtnDownload(void);
    void slotBtnTimeCount(void);
    void slotBtnNameModify(void);
    void slotBtnClockClse(void);
    void slotUpdateDisplay(void);

    bool eventFilter(QObject *watched, QEvent *event);

private:
    void initUi(void);
    void initConnect(void);
    void cleanTableWdg(void);

    Ui::StopwatchClockWdg *ui;
    ClockStatus clockNowStatus;
    NameModifyStatus nameModifyStatus;
    QTimer *updateTimer;
    QTime startTime;
    uint8_t clockIndex;
    uint8_t nowClockCount;

    QString timeStr;
    QString initTimeStr = QString("00:00:00.000");
    QString stopwatchClockName = QString("Stopwatch");
    QVector<TimeCountInfo> vTimeCount;
};

#endif  // STOPWATCHCLOCKWDG_H
