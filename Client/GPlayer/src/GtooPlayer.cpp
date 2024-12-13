#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QTextCodec>

#include "About.h"
#include "GtooPlayer.h"
#include "VideoCtrl.h"

#include "SDL.h"
// #include "SDL_main.h"

/// 由于我们建立的是C++的工程
/// 编译的时候使用的C++的编译器编译
/// 而FFMPEG是C的库
/// 因此这里需要加上extern "C"
/// 否则会提示各种未定义
extern "C" {
#include "libavcodec/avcodec.h"
}

void test(void) {
    printf("%s\n", avcodec_configuration());
}

static Uint8 *audio_chunk;
static Uint32 audio_len;
static Uint8 *audio_pos;
int pcm_buffer_size = 4096;

// 回调函数，音频设备需要更多数据的时候会调用该回调函数
void read_audio_data2(void *udata, Uint8 *stream, int len) {
    SDL_memset(stream, 0, len);
    if (audio_len == 0) return;
    len = (len > audio_len ? audio_len : len);

    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    audio_pos += len;
    audio_len -= len;
}

int testSDL() {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }

    SDL_AudioSpec spec;
    spec.freq = 44100;  // 根据你录制的PCM采样率决定
    spec.format = AUDIO_S16SYS;
    spec.channels = 1;  // 单声道
    spec.silence = 0;
    spec.samples = 1024;
    spec.callback = read_audio_data2;
    spec.userdata = NULL;

    if (SDL_OpenAudio(&spec, NULL) < 0) {
        printf("can't open audio.\n");
        return -1;
    }

    FILE *fp =
        fopen("E:\\Desktop\\languguetest\\Cplusplustest\\3-VisualStudio2017\\0-GtooPlayer\\test_video\\123.pcm", "rb+");
    if (fp == NULL) {
        printf("cannot open this file\n");
        return -1;
    }
    char *pcm_buffer = (char *)malloc(pcm_buffer_size);

    // 播放
    SDL_PauseAudio(0);

    while (1) {
        if (fread(pcm_buffer, 1, pcm_buffer_size, fp) !=
            pcm_buffer_size) {  // 从文件中读取数据，剩下的就交给音频设备去完成了，它播放完一段数据后会执行回调函数，获取等多的数据
            break;
        }

        audio_chunk = (Uint8 *)pcm_buffer;
        audio_len = pcm_buffer_size;  // 长度为读出数据长度，在read_audio_data中做减法
        audio_pos = audio_chunk;
        LOG_DEBUG("audio_len:{:d}", audio_len);

        while (audio_len > 0)  // 判断是否播放完毕
            SDL_Delay(1);
    }
    free(pcm_buffer);
    SDL_Quit();

    return 0;
}

GtooPlayer::GtooPlayer(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::GtooPlayer),
      mReadThread(new ReadThread) {
    // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); // 没有用
    // qDebug() << QString::fromLocal8Bit("中文");
    qDebug() << "中文";

    // testSDL();

    ui->setupUi(this);
    initUtils();
    initUi();
    initConnect();
}

void testLog(void) {
    LOG_INFO("中文");
    LOG_TRACE("trace");
    LOG_DEBUG("debug");
    LOG_INFO("info");
    LOG_WARN("warn");
    LOG_ERROR("error");
    LOG_CRITICAL("critical");

    LOG_INFO("PID:[{:08d}] {:08d} ", 123, 123);
    LOG_INFO("str:{}", "test");
    QString qstr("qstr");
    LOG_INFO("qstr:{}", qstr.toStdString());  // 不能直接输出QString
}

void GtooPlayer::initUtils(void) {
    testLog();
    QMediaPlayer *player = new QMediaPlayer();
    QList<QAudio::Role> ans = player->supportedAudioRoles();
    QStringList ans2 = player->supportedMimeTypes();

    // QMediaPlayer mediaPlayer;
    // QMediaContent
    // mediaPlayer->setMedia(QUrl::fromLocalFile("E:\\Desktop\\languguetest\\Cplusplustest\\3-VisualStudio2017\\0-GtooPlayer\\test_video\\123.mp3"));
    // ans = mediaPlayer.supportedAudioRoles();
    // mediaPlayer.play();
    //// 等待音频播放完成
    // while (mediaPlayer.state() == QMediaPlayer::PlayingState) {
    //     // 这里可以添加一些其他处理逻辑
    //     // 注意：在实际应用中，最好使用信号和槽机制来处理媒体状态的变化
    // }

    int t = 1;
}

void GtooPlayer::initUi(void) {
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

void GtooPlayer::initConnect(void) {
    this->setWindowTitle(mPlayerTitile);
    connect(ui->actionAbout, &QAction::triggered, this, &GtooPlayer::openAbout);
    connect(tmpExampleMenu2PlayList, &QAction::triggered, this, &GtooPlayer::openExample2PlayList);
    connect(ui->actionVideoCtrl, &QAction::triggered, this, &GtooPlayer::showVideoCtrl);

    connect(ui->actionOpen, &QAction::triggered, this, &GtooPlayer::openFile);
    connect(ui->pushButtonStart, &QPushButton::clicked, this, &GtooPlayer::startVideo);
    connect(ui->pushButtonPause, &QPushButton::clicked, this, &GtooPlayer::pauseVideo);
    connect(ui->pushButtonPrevious, &QPushButton::clicked, this, &GtooPlayer::pauseVideo);
    connect(ui->pushButtonNext, &QPushButton::clicked, this, &GtooPlayer::pauseVideo);

    connect(ui->actionRemWord, &QAction::triggered, this, &GtooPlayer::slotActionRemWord);

    // ui->play_list->currentText()
    connect(ui->playListWidget, &PlayList::SigPlay, this, &GtooPlayer::startVideoPlayList);

    // 它表示当信号被触发时，槽函数会立即在发射信号的线程上被调用。这意味着信号和槽之间的通信是直接的、同步的，不涉及事件循环的调度
    // 这是不同线程中的触发
    connect(mReadThread, &ReadThread::updateImage, ui->playImageWidget, &PlayImage::updateImage, Qt::DirectConnection);
    connect(mReadThread, &ReadThread::playState, this, &GtooPlayer::onPlayState);
    connect(mReadThread, &ReadThread::updateTime, this, &GtooPlayer::updateTime);
}

void GtooPlayer::openAbout(void) {
    About *aboutWindow = new About();
    // QWidget作为单独的显示窗口，初始化时候不能使用this
    // 如果初始化中继承了this，新建的窗口就和主窗口是同一个，拖不开了
    // About* aboutWindow = new About(this);
    aboutWindow->show();
}

void GtooPlayer::openExample2PlayList(void) {
    PlayList *exampleWindow = new PlayList();
    exampleWindow->show();
    // exampleWindow->showFullScreen();
}

void GtooPlayer::openFile(void) {
    LOG_DEBUG("openFile");
    QString filePath = QFileDialog::getOpenFileName(
        this, "选择播放视频~！", "E:/Desktop/languguetest/Cplusplustest/3-VisualStudio2017/0-GtooPlayer/test_video",
        "视频 (*.mp4 *.m4v *.mov *.avi *.flv);; 其它(*)");
    qDebug() << filePath;
    QFileInfo info(filePath);
}

void GtooPlayer::showVideoCtrl(void) {
    VideoCtrl *videoCtrlWidget = new VideoCtrl();
    videoCtrlWidget->show();
}

void GtooPlayer::startVideo(void) {
    LOG_DEBUG("startVideo");
    if (ui->pushButtonStart->text() == "开始") {
        mReadThread->open(nowPlayFilePath);
    } else {
        mReadThread->close();
    }
}

void GtooPlayer::startVideoPlayList(QString playFilePath) {
    LOG_DEBUG("startVideoPlayList");
    nowPlayFilePath = playFilePath;
    if (ui->pushButtonStart->text() == "开始") {
        mReadThread->open(nowPlayFilePath);
    } else {
        mReadThread->close();
    }
}

void GtooPlayer::slotActionRemWord(void)
{
    RemWordWdg* remWordWdg = new RemWordWdg();
    remWordWdg->show();
}

void GtooPlayer::pauseVideo(void) {
    LOG_DEBUG("pauseVideo");
}

void GtooPlayer::onPlayState(ReadThread::PlayState state) {
    if (state == ReadThread::play) {
        // this->setWindowTitle(QString("正在播放： %1").arg(mReadThread->url())); // 需要解决文件名太长的问题
        this->setWindowTitle(QString("正在播放： %1").arg("test"));
        ui->pushButtonStart->setText("停止");
    } else if (state == ReadThread::end) {
        ui->pushButtonStart->setText("开始");
        this->setWindowTitle(mPlayerTitile);
    }
}

void GtooPlayer::updateTime(QString nowTime, QString totalTime, qreal progressValue) {
    ui->labelNowTime->setText(nowTime);
    ui->labelTotalTime->setText(totalTime);

    qreal nowProgress = progressValue * (mProgressBarMax - mProgressBarMin);
    ui->progressBar->setValue(int(nowProgress));
    ui->progressBar->setFormat(QString("%1%").arg(QString::number(nowProgress, 'f', 2)));
}

GtooPlayer::~GtooPlayer() {
}
