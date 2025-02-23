#include "VideoDecode.h"

VideoFileInfo::VideoFileInfo()
    :  // TODO 没有初始化完全
      mTotalTimeMs(0),
      mNowTimeMs(0),
      mTotalTimeStamp(0),
      mNowTimeStamp(0),
      mTotalFrames(0),
      mFrameRate(0),
      mSize(0, 0),
      mCodecName() {
}

// TODO 没有清理完全
void VideoFileInfo::clear() {
    mTotalTimeMs = 0;
    mNowTimeMs = 0;
    mTotalTimeStamp = 0;
    mNowTimeStamp = 0;
    mTotalFrames = 0;
    mFrameRate = 0;
    mSize = QSize(0, 0);
    mCodecName = "";
}

QString convertMsToQString(int64_t milliseconds) {
    int seconds = milliseconds / 1000;
    int minutes = seconds / 60;
    int hours = minutes / 60;

    QString timeString = QString("%1:%2:%3")
                             .arg(hours, 2, 10, QChar('0'))
                             .arg(minutes % 60, 2, 10, QChar('0'))
                             .arg(seconds % 60, 2, 10, QChar('0'));

    return timeString;
}

void VideoFileInfo::transformTime() {
    mTotalTimeMs = mTotalTimeStamp / (AV_TIME_BASE / 1000);
    mNowTimeMs = mNowTimeStamp;

    // mTotalTimeStr = QTime::fromMSecsSinceStartOfDay(int(mTotalTimeMs)).toString("HH:mm:ss.zzz");
    // mNowTimeStr = QTime::fromMSecsSinceStartOfDay(int(mNowTimeMs)).toString("HH:mm:ss.zzz");
    mTotalTimeStr = QTime::fromMSecsSinceStartOfDay(int(mTotalTimeMs)).toString("HH:mm:ss");
    mNowTimeStr = QTime::fromMSecsSinceStartOfDay(int(mNowTimeMs)).toString("HH:mm:ss");

    mProgressValue = (qreal)mNowTimeMs / (qreal)mTotalTimeMs;
}

VideoDecode::VideoDecode()
    : mVideoFileInfo(new VideoFileInfo) {
    initFFmpeg();  // 5.1.2版本不需要调用了
}

VideoDecode::~VideoDecode() {
    close();
}

void VideoDecode::initFFmpeg() {
    static bool isFirst = true;
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if (isFirst) {
        // 注册FFmpeg的所有组件,在4.0版本以后已经被弃用,所以实际不加也可以正常编解码音视频
        // av_register_all();  // 已经从源码中删除
        avformat_network_init();
        isFirst = false;
    }
}

// 打开媒体文件,或者流媒体,例如rtmp、strp、http
bool VideoDecode::open(const QString& url) {
    if (url.isNull()) return false;

    AVDictionary* dict = nullptr;
    // 设置rtsp流使用tcp打开,如果打开失败错误信息为Error number -135 occurred
    // 可以切换(UDP、tcp、udp_multicast、http),比如vlc推流就需要使用udp打开
    av_dict_set(&dict, "rtsp_transport", "tcp", 0);

    // 设置最大复用或解复用延迟(以微秒为单位)当通过UDP接收数据时,
    // 解复用器尝试重新排序接收到的数据包(因为它们可能无序到达,或者数据包可能完全丢失)
    // 这可以通过将最大解复用延迟设置为零(通过max_delayAVFormatContext字段)来禁用
    av_dict_set(&dict, "max_delay", "3", 0);
    // 以微秒为单位设置套接字 TCP I/O 超时,如果等待时间过短,也可能会还没连接就返回了
    av_dict_set(&dict, "timeout", "1000000", 0);

    // 打开输入流并返回解封装上下文
    int ret = avformat_open_input(&mFormatContext, url.toStdString().data(), nullptr, &dict);

    if (dict) {
        av_dict_free(&dict);
    }
    // 打开视频失败
    if (ret < 0) {
        showError(ret);
        free();
        return false;
    }

    // 读取媒体文件的数据包以获取流信息
    ret = avformat_find_stream_info(mFormatContext, nullptr);
    if (ret < 0) {
        showError(ret);
        free();
        return false;
    }
    qint64 totalTime = mFormatContext->duration / (AV_TIME_BASE / 1000);  // 计算视频总时长(毫秒)
    mVideoFileInfo->mTotalTimeStamp = mFormatContext->duration;
#if PRINT_LOG
    qDebug() << QString("video total time:%1 ms,[%2]")
                    .arg(totalTime)
                    .arg(QTime::fromMSecsSinceStartOfDay(int(totalTime)).toString("HH:mm:ss zzz"));
#endif

    // 通过AVMediaType枚举查询视频流ID(也可以通过遍历查找),最后一个参数无用
    mVideoIndex = av_find_best_stream(mFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (mVideoIndex < 0) {
        showError(mVideoIndex);
        free();
        return false;
    }

    AVStream* videoStream = mFormatContext->streams[mVideoIndex];  // 通过查询到的索引获取视频流

    mAudioIndex = av_find_best_stream(mFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (mFormatContext->streams[mAudioIndex]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
        // 查找音频解码器
        const AVCodec* audioCodec = avcodec_find_decoder(mFormatContext->streams[mAudioIndex]->codecpar->codec_id);
        if (!audioCodec) {
            LOG_ERR("No audio decoder found.");
        }

        AVCodecContext* mAudioCodecContext = avcodec_alloc_context3(audioCodec);
        if (!mAudioCodecContext) {
            LOG_ERR("Unable to assign audio decoder context");
        }

        // 将参数复制到音频解码器上下文
        if (avcodec_parameters_to_context(mAudioCodecContext, mFormatContext->streams[mAudioIndex]->codecpar) < 0) {
            LOG_ERR("Unable to copy audio decoder parameters");
        }

        // 打开音频解码器
        if (avcodec_open2(mAudioCodecContext, audioCodec, NULL) < 0) {
            LOG_ERR("Unable to open audio decoder");
        }
    }

    // 获取视频图像分辨率(AVStream中的AVCodecContext在新版本中弃用,改为使用AVCodecParameters)
    QSize mSize;
    mSize.setWidth(videoStream->codecpar->width);
    mSize.setHeight(videoStream->codecpar->height);
    qreal mFrameRate;
    mFrameRate = rationalToDouble(&videoStream->avg_frame_rate);  // 视频帧率

    // 通过解码器ID获取视频解码器(新版本返回值必须使用const)
    const AVCodec* codec = avcodec_find_decoder(videoStream->codecpar->codec_id);
    int64_t mTotalFrames;
    mTotalFrames = videoStream->nb_frames;

    mVideoFileInfo->mSize = mSize;
    mVideoFileInfo->mFrameRate = mFrameRate;
    mVideoFileInfo->mTotalFrames = mTotalFrames;
    mVideoFileInfo->mCodecName = QString(codec->name);

    LOG_DBG("resolution :[w:{},h:{}] framse:{}  ", mSize.width(), mSize.height(), mFrameRate);
    LOG_DBG("total framse: {}  codec: {}", mTotalFrames, codec->name);

    // 分配AVCodecContext并将其字段设置为默认值
    mCodecContext = avcodec_alloc_context3(codec);
    if (!mCodecContext) {
        LOG_ERR("Failed to create video decoder context!");
        free();
        return false;
    }

    // 使用视频流的codecpar为解码器上下文赋值
    ret = avcodec_parameters_to_context(mCodecContext, videoStream->codecpar);
    if (ret < 0) {
        showError(ret);
        free();
        return false;
    }

    mCodecContext->flags2 |= AV_CODEC_FLAG2_FAST;  // 允许不符合规范的加速技巧
    mCodecContext->thread_count = 8;               // 使用8线程解码

    // 初始化解码器上下文,如果之前avcodec_alloc_context3传入了解码器,这里设置NULL就可以
    ret = avcodec_open2(mCodecContext, nullptr, nullptr);
    if (ret < 0) {
        showError(ret);
        free();
        return false;
    }

    // 分配AVPacket并将其字段设置为默认值
    mPacket = av_packet_alloc();
    if (!mPacket) {
        qWarning() << "av_packet_alloc() Error!";
        free();
        return false;
    }
    // 分配AVFrame并将其字段设置为默认值
    mFrame = av_frame_alloc();
    if (!mFrame) {
        LOG_WRN("av_frame_alloc() Error!");
        free();
        return false;
    }

    mAudioFrame = av_frame_alloc();
    if (!mAudioFrame) {
        LOG_WRN("av_frame_alloc() Error!");
        free();
        return false;
    }

    // 分配图像空间
    int size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, mSize.width(), mSize.height(), 4);

    // 【注意:】这里可以多分配一些,否则如果只是安装size分配,大部分视频图像数据拷贝没有问题,
    //          但是少部分视频图像在使用sws_scale()拷贝时会超出数组长度,在使用使用msvc debug模式时delete[]
    //  mBuffer会报错(HEAP CORRUPTION DETECTED: after Normal block(#32215) at 0x000001AC442830370.CRT delected that
    //  the
    // application wrote to memory after end of heap buffer)
    //         特别是这个视频流http://vfx.mtime.cn/Video/2019/02/04/mp4/190204084208765161.mp4

    // 这里多分配1000个字节就基本不会出现拷贝超出的情况了,反正不缺这点内存
    // m_image = new QImage(mBuffer, mSize.width(), mSize.height(), QImage::Format_RGBA8888);
    //    这种方式分配内存大部分情况下也可以,但是因为存在拷贝超出数组的情况,delete时也会报错
    mBuffer = new uchar[size + 1000];

    mEnd = false;
    return true;
}

QImage VideoDecode::read() {
    // 如果没有打开则返回
    if (!mFormatContext) {
        return QImage();
    }

    // 读取下一帧数据
    int readRet = av_read_frame(mFormatContext, mPacket);
    if (readRet < 0) {
        avcodec_send_packet(mCodecContext, mPacket);  // 读取完成后向解码器中传如空AVPacket,否则无法读取出最后几帧
    } else {
        if (mPacket->stream_index == mVideoIndex)  // 如果是图像数据则进行解码
        {
            AVRational time_base = mFormatContext->streams[mVideoIndex]->time_base;
            // 计算当前帧时间(毫秒)
            // 方法一:适用于所有场景,但是存在一定误差
            mPacket->pts = qRound64(mPacket->pts * (1000 * rationalToDouble(&time_base)));
            mPacket->dts = qRound64(mPacket->dts * (1000 * rationalToDouble(&time_base)));

            // 方法二:适用于播放本地视频文件,计算每一帧时间较准,但是由于网络视频流无法获取总帧数,所以无法适用
            // mObtainFrames++;
            // mPacket->pts = qRound64(mObtainFrames * (qreal(m_totalTime) / m_totalFrames));

            // 将读取到的原始数据包传入解码器
            int ret = avcodec_send_packet(mCodecContext, mPacket);
            if (ret < 0) {
                showError(ret);
            }
        }
    }
    av_packet_unref(mPacket);  // 释放数据包,引用计数-1,为0时释放空间

    int ret = avcodec_receive_frame(mCodecContext, mFrame);
    if (ret < 0) {
        av_frame_unref(mFrame);
        if (readRet < 0) {
            mEnd = true;  // 当无法读取到AVPacket并且解码器中也没有数据时表示读取完成
        }
        return QImage();
    }

    mVideoFileInfo->mNowTimeStamp = mFrame->best_effort_timestamp;
    mVideoFileInfo->transformTime();

    LOG_DBG("Ms-{}:{} Stamp-{}:{} ", mVideoFileInfo->mNowTimeMs, mVideoFileInfo->mTotalTimeMs,
            mVideoFileInfo->mNowTimeStamp, mVideoFileInfo->mTotalTimeStamp);
    LOG_DBG("Str-{}/{} Progress:{} ", mVideoFileInfo->mNowTimeStr.toStdString(),
            mVideoFileInfo->mTotalTimeStr.toStdString(), mVideoFileInfo->mProgressValue);
    // qDebug() << QString("Ms-%1:%2 Stamp-%3:%4 Str-%5/%6 Progress:%7").
    //     arg(mVideoFileInfo->mNowTimeMs).arg(mVideoFileInfo->mTotalTimeMs).
    //     arg(mVideoFileInfo->mNowTimeStamp).arg(mVideoFileInfo->mTotalTimeStamp).
    //     arg(mVideoFileInfo->mNowTimeStr).arg(mVideoFileInfo->mTotalTimeStr).arg(mVideoFileInfo->mProgressValue);

    mPts = mFrame->pts;

    // 为什么图像转换上下文要放在这里初始化呢,是因为mFrame->format,如果使用硬件解码,解码出来的图像格式和mCodecContext->pix_fmt的图像格式不一样,就会导致无法转换为QImage
    if (!mSwsContext) {
        // 获取缓存的图像转换上下文首先校验参数是否一致,如果校验不通过就释放资源；然后判断上下文是否存在,如果存在直接复用,如不存在进行分配、初始化操作
        mSwsContext = sws_getCachedContext(
            mSwsContext,
            mFrame->width,                   // 输入图像的宽度
            mFrame->height,                  // 输入图像的高度
            (AVPixelFormat)mFrame->format,   // 输入图像的像素格式
            mVideoFileInfo->mSize.width(),   // 输出图像的宽度
            mVideoFileInfo->mSize.height(),  // 输出图像的高度
            AV_PIX_FMT_RGBA,                 // 输出图像的像素格式
            SWS_BILINEAR,  // 选择缩放算法(只有当输入输出图像大小不同时有效),一般选择SWS_FAST_BILINEAR
            nullptr,   // 输入图像的滤波器信息, 若不需要传NULL
            nullptr,   // 输出图像的滤波器信息, 若不需要传NULL
            nullptr);  // 特定缩放算法需要的参数(?),默认为NULL
        if (!mSwsContext) {
            LOG_WRN("sws_getCachedContext() Error!");
            free();
            return QImage();
        }
    }

    // AVFrame转QImage
    uchar* data[] = {mBuffer};
    int lines[4];
    av_image_fill_linesizes(lines, AV_PIX_FMT_RGBA,
                            mFrame->width);  // 使用像素格式pix_fmt和宽度填充图像的平面线条大小
    ret = sws_scale(mSwsContext,             // 缩放上下文
                    mFrame->data,            // 原图像数组
                    mFrame->linesize,        // 包含源图像每个平面步幅的数组
                    0,                       // 开始位置
                    mFrame->height,          // 行数
                    data,                    // 目标图像数组
                    lines);                  // 包含目标图像每个平面的步幅的数组
    QImage image(mBuffer, mFrame->width, mFrame->height, QImage::Format_RGBA8888);
    av_frame_unref(mFrame);

    return image;
}

// 关闭视频播放并释放内存
void VideoDecode::close() {
    clear();
    free();

    mVideoFileInfo->clear();
    mObtainFrames = 0;
    mPts = 0;
}

// 视频是否读取完成
bool VideoDecode::isEnd() {
    return mEnd;
}

// 返回当前帧图像播放时间
const qint64& VideoDecode::pts() {
    return mPts;
}

// 显示ffmpeg函数调用异常信息
void VideoDecode::showError(int err) {
    char* mError = new char[ERROR_LEN];
    memset(mError, 0, ERROR_LEN);
    av_strerror(err, mError, ERROR_LEN);
    LOG_ERR("DecodeVideo Error: {:d}", mError);
}

// 将AVRational转换为double,用于计算帧率
qreal VideoDecode::rationalToDouble(AVRational* rational) {
    qreal frameRate = (rational->den == 0) ? 0 : (qreal(rational->num) / rational->den);
    return frameRate;
}

// 清空读取缓冲
void VideoDecode::clear() {
    // 因为avformat_flush不会刷新AVIOContext (s->pb)如果有必要,在调用此函数之前调用avio_flush(s->pb)
    if (mFormatContext && mFormatContext->pb) {
        avio_flush(mFormatContext->pb);
    }
    if (mFormatContext) {
        avformat_flush(mFormatContext);  // 清理读取缓冲
    }
}

void VideoDecode::free() {
    // 释放上下文swsContext
    if (mSwsContext) {
        sws_freeContext(mSwsContext);
        mSwsContext = nullptr;  // sws_freeContext不会把上下文置NULL
    }
    // 释放编解码器上下文和与之相关的所有内容,并将NULL写入提供的指针
    if (mCodecContext) {
        avcodec_free_context(&mCodecContext);
    }
    // 关闭并失败mFormatContext,并将指针置为null
    if (mFormatContext) {
        avformat_close_input(&mFormatContext);
    }
    if (mPacket) {
        av_packet_free(&mPacket);
    }
    if (mFrame) {
        av_frame_free(&mFrame);
    }
    if (mBuffer) {
        delete[] mBuffer;
        mBuffer = nullptr;
    }
}
