#ifndef VIDEODECODE_H
#define VIDEODECODE_H

#include "global.h"

struct AVFormatContext;
struct AVCodecContext;
struct AVRational;
struct AVPacket;
struct AVFrame;
struct SwsContext;
struct AVBufferRef;
class QImage;

#define ERROR_LEN 1024  // 异常信息数组长度

class VideoFileInfo {
public:
    VideoFileInfo();
    ~VideoFileInfo() = default;

    void clear();
    void transformTime();

public:
    int64_t mTotalTimeStamp = 0;  // 视频总时长
    int64_t mTotalTimeMs = 0;
    QString mTotalTimeStr = 0;
    int64_t mNowTimeStamp = 0;    // 现在播放的时间
    int64_t mNowTimeMs = 0;
    QString mNowTimeStr = 0;
    qreal mProgressValue = 0;

    qint64 mTotalFrames = 0;  // 视频总帧数
    qreal mFrameRate = 0;     // 视频帧率
    QSize mSize;              // 视频分辨率大小
    QString mCodecName;       // 解码器名字
};

class VideoDecode {
public:
    VideoDecode();
    ~VideoDecode();

    bool open(const QString& url = QString());  // 打开媒体文件，或者流媒体rtmp、strp、http
    QImage read();                              // 读取视频图像
    void close();                               // 关闭
    bool isEnd();                               // 是否读取完成
    const qint64& pts();                        // 获取当前帧显示时间

public:
    VideoFileInfo* mVideoFileInfo;

private:
    void initFFmpeg();                             // 初始化ffmpeg库（整个程序中只需加载一次）
    void showError(int err);                       // 显示ffmpeg执行错误时的错误信息
    qreal rationalToDouble(AVRational* rational);  // 将AVRational转换为double
    void clear();                                  // 清空读取缓冲
    void free();                                   // 释放

private:
    AVFormatContext* mFormatContext = nullptr;     // 解封装上下文
    AVCodecContext* mCodecContext = nullptr;       // 解码器上下文
    SwsContext* mSwsContext = nullptr;             // 图像转换上下文
    AVPacket* mPacket = nullptr;                   // 数据包
    AVFrame* mFrame = nullptr;                     // 解码后的视频帧
    int mVideoIndex = 0;                           // 视频流索引
    qint64 mObtainFrames = 0;                      // 视频当前获取到的帧数
    qint64 mPts = 0;                               // 图像帧的显示时间

    bool mEnd = false;                             // 视频读取完成
    uchar* mBuffer = nullptr;  // YUV图像需要转换位RGBA图像，这里保存转换后的图形数据

    AVCodecContext* mAudioCodecContext = nullptr;
    int mAudioIndex = 0;
    AVFrame* mAudioFrame = nullptr;  // 解码后的视频帧
};

#endif                               // VIDEODECODE_H
