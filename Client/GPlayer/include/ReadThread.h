#ifndef READTHREAD_H
#define READTHREAD_H

#include <QElapsedTimer>
#include <QThread>
#include <QTime>
#include "global.h"

class VideoDecode;
class VoiceDecode;

class ReadThread : public QThread {
    Q_OBJECT
public:
    enum PlayState  // 视频播放状态
    {
        play,
        end
    };

public:
    explicit ReadThread(QObject* parent = nullptr);
    ~ReadThread() override;

    void open(const QString& url = QString());  // 打开视频
    void pause(bool flag);                      // 暂停视频
    void close();                               // 关闭视频
    const QString& url();                       // 获取打开的视频地址

protected:
    void run() override;

signals:
    void updateImage(const QImage& image);  // 将读取到的视频图像发送出去
    void playState(PlayState state);        // 视频播放状态发送改变时触发
    void updateTime(const QString& nowTimeStr, const QString totalTimeStr, const qreal progressValue);

private:
    VideoDecode* mVideoDecode = nullptr;  // 视频解码类
    QString mUrl;                         // 打开的视频地址
    bool mPlay = false;                   // 播放控制
    bool mPause = false;                  // 暂停控制
    QElapsedTimer mTime1;                 // 控制视频播放速度（更精确，但不支持视频后退）
    QTime mTime2;                         // 控制视频播放速度（支持视频后退）
};

class ReadVideoThread : public QThread {
    Q_OBJECT
public:
    enum PlayState {
        play,
        end
    };

    explicit ReadVideoThread(QObject* parent = nullptr);
    ~ReadVideoThread() override;
    void open(const QString& url = QString());  // 打开视频

protected:
    void run() override;

private:
    std::shared_ptr<VoiceDecode> mspVoiceDecode = nullptr;
    QString mUrl;
};

#endif  // READTHREAD_H
