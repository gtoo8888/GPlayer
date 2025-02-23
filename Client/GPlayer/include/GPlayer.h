#pragma once

#include "RemWordWdg.h"
#include "global.h"
#include "ui_GPlayer.h"

class Version {
public:
    static const uint8 major = 0;
    static const uint8 minor = 0;
    static const uint8 patch = 4;

    static std::string getVersionStr(void) {
        char version[15];
        sprintf(version, "v%i.%i.%i", major, minor, patch);
        return std::string(version);
    }
};

class GPlayer : public QMainWindow {
    Q_OBJECT

public:
    GPlayer(QWidget* parent = nullptr);
    ~GPlayer() = default;

    QPushButton* buttonOccupy;         // TODE 临时demo
    QMenu* tmpExampleMenu;             // TODE 临时demo
    QAction* tmpExampleMenu2PlayList;  // TODE 临时demo

    QMenu m_stMenu;
    QMenu* stRemoveMenu;
    QAction m_stActAdd;        // 添加文件
    QAction m_stActRemove;     // 移除文件
    QAction m_stActClearList;  // 清空列表

public slots:
    void slotOpenAbout(void);
    void slotOpenFile(void);
    void slotOpenExample2PlayList(void);
    void slotShowVideoCtrl(void);

    void slotPauseVideo(void);
    void slotStartVideo(void);
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
    Ui::GPlayer* ui;
    std::string msPlayerTitile;
    std::shared_ptr<ReadThread> mspReadThread;
    std::shared_ptr<ReadVoiceThread> mspReadVoiceThread;
    int64_t mProgressBarMin = 0;
    int64_t mProgressBarMax = 100;
    QString nowPlayFilePath = QString("");
};
