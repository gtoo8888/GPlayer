#include "ReadThread.h"
#include "VideoDecode.h"
#include "VoiceDecode.h"

#include <qimage.h>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>

ReadThread::ReadThread(QObject *parent)
    : QThread(parent) {
    mVideoDecode = new VideoDecode();
    mVoiceDecode = new VoiceDecode();

    qRegisterMetaType<PlayState>("PlayState");  // 注册自定义枚举类型，否则信号槽无法发送
}

ReadThread::~ReadThread() {
    if (mVideoDecode) {
        delete mVideoDecode;
    }
}

void ReadThread::open(const QString &url) {
    if (!this->isRunning()) {
        m_url = url;
        emit this->start();  
    }
}


void ReadThread::pause(bool flag) {
    m_pause = flag;
}

void ReadThread::close() {
    m_play = false;
    m_pause = false;
}

const QString &ReadThread::url() {
    return m_url;
}

// 非阻塞延时
void sleepMsec(int msec) {
    if (msec <= 0) return;
    QEventLoop loop;                                // 定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));  // 创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();  // 事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void ReadThread::run() {
    bool ret = mVideoDecode->open(m_url);  // 打开网络流时会比较慢，如果放到Ui线程会卡
    ret = mVoiceDecode->open(m_url);
    if (ret) {
        m_play = true;
        m_etime1.start();
        m_etime2.start();
        emit playState(play);
    } else {
        qWarning() << "打开失败！";
    }
    // 循环读取视频图像
    while (m_play) {
        // 暂停
        while (m_pause) {
            sleepMsec(200);
        }

        QImage image = mVideoDecode->read();  // 读取视频图像
        if (!image.isNull()) {
            // 1倍速播放
#if 0
            sleepMsec(int(m_decodeVideo->pts() - m_etime1.elapsed()));         // 不支持后退
#else
            sleepMsec(int(mVideoDecode->pts() -
                          m_etime2.elapsed()));  // 支持后退（如果能读取到视频，但一直不显示可以把这一行代码注释试试）
#endif
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

    qDebug() << "播放结束！";
    mVideoDecode->close();
    emit playState(end);
}
