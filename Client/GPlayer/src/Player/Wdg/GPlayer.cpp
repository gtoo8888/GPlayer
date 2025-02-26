#include "GPlayer.h"

GPlayer::GPlayer(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::GPlayer) {
    ui->setupUi(this);

    std::stringstream ss;
    ss << "GPlayer " << Version::getVersionStr();
    msPlayerTitile = ss.str();
    // LOG_DBG("{:s}\n", avcodec_configuration());
    LOG_INF("{:s}\n", msPlayerTitile);

    mspReadThread = std::make_shared<ReadThread>();
    mspReadVoiceThread = std::make_shared<ReadVoiceThread>();
    mspVideoCtrlWdg = std::make_shared<VideoCtrlWdg>();
    // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); // 没有用
    // qDebug() << QString::fromLocal8Bit("中文");

    initUi();
    initConnect();
}

void GPlayer::initUi(void) {
    this->setWindowTitle(QString::fromStdString(msPlayerTitile));
    // 保留一下添加toolbar的方式
    // buttonOccupy = new QPushButton(QString::fromLocal8Bit("about"));
    // ui->ToolBar->addWidget(buttonOccupy);

    mvMenuName = {"文件", "视图", "控制", "帮助", "应用"};
    for (const std::string &menuName : mvMenuName) {
        QMenu *menu = new QMenu(QString::fromStdString(menuName), this);
        mvMenu.push_back(menu);
        ui->menuBar->addMenu(menu);
    }

    mvFileOperationName = {"打开", "关闭", "退出", "播放列表"};
    for (const std::string &actName : mvFileOperationName) {
        QAction *action = new QAction(QString::fromStdString(actName), this);
        mvActFileOperation.push_back(action);
        mvMenu[MenuType::FILE_OPE]->addAction(action);
    }

    mvViewName = {"统计信息", "全屏"};
    for (const std::string &actName : mvViewName) {
        QAction *action = new QAction(QString::fromStdString(actName), this);
        mvActView.push_back(action);
        mvMenu[MenuType::VIEW]->addAction(action);
    }
    menuZoom = new QMenu(QString::fromStdString("缩放"), this);
    mvMenu[MenuType::VIEW]->addMenu(menuZoom);

    mvViewZoomName = {"50%", "100%", "200%"};
    for (const std::string &actName : mvViewZoomName) {
        QAction *action = new QAction(QString::fromStdString(actName), this);
        mvActViewZoom.push_back(action);
        menuZoom->addAction(action);
    }

    mvCtrlName = {"播放/暂停", "停止", "单帧步进", "单帧后退", "第一帧", "最末帧", "定位", "视频设置"};
    for (const std::string &actName : mvCtrlName) {
        QAction *action = new QAction(QString::fromStdString(actName), this);
        mvActCtrl.push_back(action);
        mvMenu[MenuType::CTRL]->addAction(action);
    }

    mvHelpName = {"操作说明", "关于"};
    for (const std::string &actName : mvHelpName) {
        QAction *action = new QAction(QString::fromStdString(actName), this);
        mvActHelp.push_back(action);
        mvMenu[MenuType::HRLP]->addAction(action);
    }

    mvAppName = {"背单词", "秒表"};
    for (const std::string &actName : mvAppName) {
        QAction *action = new QAction(QString::fromStdString(actName), this);
        mvActApp.push_back(action);
        mvMenu[MenuType::APP]->addAction(action);
    }

    // ui->play_list->setEditable(true); // QComboBox需要开启才能编辑

    ui->progressBar->setMinimum(mProgressBarMin);
    ui->progressBar->setMaximum(mProgressBarMax);
    ui->progressBar->setValue(mProgressBarMin);
}

void GPlayer::initConnect(void) {
    connect(mvActFileOperation[FileOperationType::OPEN], &QAction::triggered, this, &GPlayer::slotOpenFile);
    connect(mvActFileOperation[FileOperationType::PALY_LIST], &QAction::triggered, this,
            &GPlayer::slotOpenExample2PlayList);

    connect(mvActCtrl[VideoCtrlType::SETUP], &QAction::triggered, this, &GPlayer::slotShowVideoCtrlWdg);

    connect(mvActHelp[HelpType::ABOUT], &QAction::triggered, this, &GPlayer::slotOpenAbout);

    connect(mvActApp[AppType::REM_WORD], &QAction::triggered, this, &GPlayer::slotActionRemWord);

    connect(ui->btnStart, &QPushButton::clicked, this, &GPlayer::slotStartVideo);
    connect(ui->btnPause, &QPushButton::clicked, this, &GPlayer::slotPauseVideo);
    connect(ui->btnPrevious, &QPushButton::clicked, this, &GPlayer::slotPauseVideo);
    connect(ui->btnNext, &QPushButton::clicked, this, &GPlayer::slotPauseVideo);

    // ui->play_list->currentText()
    // connect(ui->playListWidget, &PlayList::sigPlay, this, &GPlayer::startVideoPlayList);

    connect(mspReadThread.get(), &ReadThread::updateImage, ui->playImageWidget, &PlayImage::updateImage,
            Qt::DirectConnection);
    connect(mspReadThread.get(), &ReadThread::playState, this, &GPlayer::onPlayState);
    connect(mspReadThread.get(), &ReadThread::updateTime, this, &GPlayer::updateTime);

    connect(mspVideoCtrlWdg.get(), &VideoCtrlWdg::sigSldBrightnessChanged, this, &GPlayer::slotVideoCtrlBrtChanged);
    connect(mspVideoCtrlWdg.get(), &VideoCtrlWdg::sigSldContrastChanged, this, &GPlayer::slotVideoCtrlBrtChanged);
    connect(mspVideoCtrlWdg.get(), &VideoCtrlWdg::sigSldSaturationChanged, this, &GPlayer::slotVideoCtrlBrtChanged);
    connect(mspVideoCtrlWdg.get(), &VideoCtrlWdg::sigSldColorChanged, this, &GPlayer::slotVideoCtrlBrtChanged);
}

void GPlayer::slotOpenAbout(void) {
    About *aboutWdg = new About();
    aboutWdg->show();
}

void GPlayer::slotOpenExample2PlayList(void) {
    PlayList *playListWdg = new PlayList();
    playListWdg->show();
}

void GPlayer::slotOpenFile(void) {
    QString filePath = QFileDialog::getOpenFileName(this, "Select Play Video", "G:\download",
                                                    "Video (*.mp4 *.m4v *.mov *.avi *.flv);; Other(*)");
    QFileInfo info(filePath);
    nowPlayFilePath = filePath;
}

void GPlayer::slotShowVideoCtrlWdg(void) {
    mspVideoCtrlWdg->show();
}

void GPlayer::slotStartVideo(void) {
    if (ui->btnStart->text() == "start") {
        mspReadThread->open(nowPlayFilePath);
        mspReadVoiceThread->open(nowPlayFilePath);
    } else {
        mspReadThread->close();
    }
}

void GPlayer::startVideoPlayList(QString playFilePath) {
    nowPlayFilePath = playFilePath;
    if (ui->btnStart->text() == "start") {
        mspReadThread->open(nowPlayFilePath);
    } else {
        mspReadThread->close();
    }
}

void GPlayer::slotActionRemWord(void) {
    RemWordWdg *remWordWdg = new RemWordWdg();
    remWordWdg->show();
}

void GPlayer::slotVideoCtrlBrtChanged(int32 value) {
    LOG_DBG("sld Brightness {:d}", value);
}

void GPlayer::slotPauseVideo(void) {
}

void GPlayer::onPlayState(ReadThread::PlayState state) {
    if (state == ReadThread::play) {
        // this->setWindowTitle(QString("正在播放： %1").arg(mspReadThread->url())); // 需要解决文件名太长的问题
        this->setWindowTitle(QString("be playing: %1").arg("test"));
        ui->btnStart->setText("stop");
    } else if (state == ReadThread::end) {
        ui->btnStart->setText("start");
    }
}

void GPlayer::updateTime(QString nowTime, QString totalTime, qreal progressValue) {
    ui->labelNowTime->setText(nowTime);
    ui->labelTotalTime->setText(totalTime);

    qreal nowProgress = progressValue * (mProgressBarMax - mProgressBarMin);
    ui->progressBar->setValue(int(nowProgress));
    ui->progressBar->setFormat(QString("%1%").arg(QString::number(nowProgress, 'f', 2)));
}
