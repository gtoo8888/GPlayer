#include "ReadThread.h"

ReadThread::ReadThread(QObject *parent)
    : QThread(parent) {
    mVideoDecode = new VideoDecode();

    qRegisterMetaType<PlayState>("PlayState");  // 注册自定义枚举类型，否则信号槽无法发送
}

ReadThread::~ReadThread() {
    if (mVideoDecode) {
        delete mVideoDecode;
    }
}

void ReadThread::open(const QString &url) {
    if (!this->isRunning()) {
        mUrl = url;
        emit this->start();
    }
}

void ReadThread::pause(bool flag) {
    mPause = flag;
}

void ReadThread::close() {
    mPlay = false;
    mPause = false;
}

const QString &ReadThread::url() {
    return mUrl;
}

// 非阻塞延时
void sleepMsec(int msec) {
    if (msec <= 0) return;
    QEventLoop loop;                                // 定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));  // 创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();  // 事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void ReadThread::run() {
    bool ret = mVideoDecode->open(mUrl);  // 打开网络流时会比较慢，如果放到Ui线程会卡
    // ret = mVoiceDecode->open(mUrl);
    if (ret) {
        mPlay = true;
        mTime1.start();
        mTime2.start();
        emit playState(play);
    } else {
        LOG_WRN("open fail");
    }
    // 循环读取视频图像
    while (mPlay) {
        // 暂停
        while (mPause) {
            sleepMsec(200);
        }

        QImage image = mVideoDecode->read();  // 读取视频图像
        if (!image.isNull()) {
            // 1倍速播放

            sleepMsec(int(mVideoDecode->pts() - mTime1.elapsed()));  // 不支持后退

            // 支持后退（如果能读取到视频，但一直不显示可以把这一行代码注释试试）
            // sleepMsec(int(mVideoDecode->pts() - mTime2.elapsed()));

            emit updateImage(image);
            emit updateTime(mVideoDecode->mVideoFileInfo->mNowTimeStr, mVideoDecode->mVideoFileInfo->mTotalTimeStr,
                            mVideoDecode->mVideoFileInfo->mProgressValue);
        } else {
            // 当前读取到无效图像时判断是否读取完成
            if (mVideoDecode->isEnd()) {
                break;
            }
            sleepMsec(1);  // 这里不能使用QThread::msleep()延时，否则会很不稳定
        }
    }

    qDebug() << "play finish";
    mVideoDecode->close();
    emit playState(end);
}

ReadVoiceThread::ReadVoiceThread(QObject *parent) {
    mspVoiceDecode = std::make_shared<VoiceDecode>();
}

ReadVoiceThread::~ReadVoiceThread() {
}

void ReadVoiceThread::open(const QString &url) {
    if (!this->isRunning()) {
        mUrl = url;
        emit this->start();
    }
}

void ReadVoiceThread::run() {
    bool ret = mspVoiceDecode->openVoiceUrl(mUrl.toStdString());
}
