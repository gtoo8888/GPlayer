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

struct VideoAction {
    std::string text;
    std::function<void()> slotFunction;
};

typedef enum {
    FILE_OPE = 0,
    VIEW,
    CTRL,
    HRLP,
    APP
} MenuType;

typedef enum {
    OPEN = 0,
    CLOSE,
    EXIT,
    PALY_LIST
} FileOperationType;

typedef enum {
    STATS_INFO = 0,
    FULL_SCREEN,
    ZOOM
} ViewType;

typedef enum {
    ZOOM_050 = 0,
    ZOOM_100,
    ZOOM_200
} ViewZoomType;

typedef enum {
    PLAY_PAUSE = 0,
    STOP,
    SINGLE_FRAME_STEP,
    SINGLE_FRAME_BACK,
    FIRST_FRAME,
    LAST_FRAME,
    VIDEO_LOCATION,
    SETUP,
} VideoCtrlType;

typedef enum {
    OPT_GUIDE = 0,
    ABOUT,
} HelpType;

typedef enum {
    REM_WORD = 0,
    STOP_CLOCK,
} AppType;

class GPlayer : public QMainWindow {
    Q_OBJECT

public:
    GPlayer(QWidget* parent = nullptr);
    ~GPlayer() = default;

    QPushButton* buttonOccupy;  // TODE 临时demo

    QMenu* menuZoom;

    std::vector<std::string> mvMenuName;
    std::vector<std::string> mvFileOperationName;
    std::vector<std::string> mvViewName;
    std::vector<std::string> mvViewZoomName;
    std::vector<std::string> mvCtrlName;
    std::vector<std::string> mvHelpName;
    std::vector<std::string> mvAppName;
    std::vector<QMenu*> mvMenu;
    std::vector<QAction*> mvActFileOperation;
    std::vector<QAction*> mvActView;
    std::vector<QAction*> mvActViewZoom;
    std::vector<QAction*> mvActCtrl;
    std::vector<QAction*> mvActHelp;
    std::vector<QAction*> mvActApp;

public slots:
    void slotOpenAbout(void);
    void slotOpenFile(void);
    void slotOpenExample2PlayList(void);
    void slotShowVideoCtrlWdg(void);

    void slotPauseVideo(void);
    void slotStartVideo(void);
    void startVideoPlayList(QString playFilePath = "");

    void slotActionRemWord(void);

    void slotVideoCtrlBrtChanged(int32 value);


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
    
    std::shared_ptr<VideoCtrlWdg> mspVideoCtrlWdg;
};
