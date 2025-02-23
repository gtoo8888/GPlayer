#include "GPlayer.h"

GPlayer::GPlayer(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::GPlayer) {
    ui->setupUi(this);

    std::stringstream ss;
    ss << "GPlayer " << Version::getVersionStr();
    msPlayerTitile = ss.str();
    //LOG_DBG("{:s}\n", avcodec_configuration());
    LOG_INF("{:s}\n", msPlayerTitile);

    mspReadThread = std::make_shared<ReadThread>();
    mspReadVoiceThread = std::make_shared<ReadVoiceThread>();
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

    tmpExampleMenu = ui->menuBar->addMenu("example");  // 创建例子
    tmpExampleMenu2PlayList = tmpExampleMenu->addAction("playlist");

    // ui->play_list->setEditable(true); // QComboBox需要开启才能编辑

    ui->progressBar->setMinimum(mProgressBarMin);
    ui->progressBar->setMaximum(mProgressBarMax);
    ui->progressBar->setValue(mProgressBarMin);
}

void GPlayer::initConnect(void) {
    connect(ui->actionAbout, &QAction::triggered, this, &GPlayer::slotOpenAbout);
    connect(tmpExampleMenu2PlayList, &QAction::triggered, this, &GPlayer::slotOpenExample2PlayList);
    connect(ui->actionVideoCtrl, &QAction::triggered, this, &GPlayer::slotShowVideoCtrl);

    connect(ui->actionOpen, &QAction::triggered, this, &GPlayer::slotOpenFile);
    connect(ui->btnStart, &QPushButton::clicked, this, &GPlayer::slotStartVideo);
    connect(ui->btnPause, &QPushButton::clicked, this, &GPlayer::slotPauseVideo);
    connect(ui->btnPrevious, &QPushButton::clicked, this, &GPlayer::slotPauseVideo);
    connect(ui->btnNext, &QPushButton::clicked, this, &GPlayer::slotPauseVideo);

    connect(ui->actionRemWord, &QAction::triggered, this, &GPlayer::slotActionRemWord);

    // ui->play_list->currentText()
    // connect(ui->playListWidget, &PlayList::sigPlay, this, &GPlayer::startVideoPlayList);

    // 它表示当信号被触发时，槽函数会立即在发射信号的线程上被调用。这意味着信号和槽之间的通信是直接的、同步的，不涉及事件循环的调度
    // 这是不同线程中的触发
    connect(mspReadThread.get(), &ReadThread::updateImage, ui->playImageWidget, &PlayImage::updateImage,
            Qt::DirectConnection);
    connect(mspReadThread.get(), &ReadThread::playState, this, &GPlayer::onPlayState);
    connect(mspReadThread.get(), &ReadThread::updateTime, this, &GPlayer::updateTime);
}

void GPlayer::slotOpenAbout(void) {
    About *aboutWindow = new About();
    // QWidget作为单独的显示窗口，初始化时候不能使用this,如果初始化中继承了this，新建的窗口就和主窗口是同一个，拖不开了
    // About* aboutWindow = new About(this);
    aboutWindow->show();
}

void GPlayer::slotOpenExample2PlayList(void) {
    PlayList *exampleWindow = new PlayList();
    exampleWindow->show();
    // exampleWindow->showFullScreen();
}

void GPlayer::slotOpenFile(void) {
    QString filePath = QFileDialog::getOpenFileName(this, "Select Play Video", "G:\download",
                                                    "Video (*.mp4 *.m4v *.mov *.avi *.flv);; Other(*)");
    QFileInfo info(filePath);
    nowPlayFilePath = filePath;
}

void GPlayer::slotShowVideoCtrl(void) {
    VideoCtrl *videoCtrlWidget = new VideoCtrl();
    videoCtrlWidget->show();
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
