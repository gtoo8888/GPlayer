#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GtooPlayer.h"
#include "ReadThread.h"
#include "PlayList.h"
#include "GtooLogger.h"
#include "RemWordWdg.h"

class GtooPlayer : public QMainWindow
{
    Q_OBJECT

public:
    GtooPlayer(QWidget *parent = nullptr);
    ~GtooPlayer();

    QPushButton* buttonOccupy; // TODE 临时demo
    QMenu* tmpExampleMenu; // TODE 临时demo
    QAction* tmpExampleMenu2PlayList;  // TODE 临时demo

    QMenu m_stMenu;
    QMenu* stRemoveMenu;
    QAction m_stActAdd;     //添加文件
    QAction m_stActRemove;  //移除文件
    QAction m_stActClearList;//清空列表

public slots:
    void openAbout(void);
    void openFile(void);
    void openExample2PlayList(void);
    void showVideoCtrl(void);

    void pauseVideo(void);
    void startVideo(void);
    void startVideoPlayList(QString playFilePath = "");

    void slotActionRemWord(void);

private:
    void initUtils(void);
    void initConnect(void);
    void initUi(void);
    void onPlayState(ReadThread::PlayState state);
    void updateTime(QString nowTime, QString totalTime, qreal progressValue);
    void updateSlider(int64_t);

private:
    Ui::GtooPlayer* ui;
    QString mPlayerTitile = QString("我的播放器-GtooPlay");
    ReadThread* mReadThread = nullptr;
    int64_t mProgressBarMin = 0;
    int64_t mProgressBarMax = 100;
    QString nowPlayFilePath = QString("");
};
