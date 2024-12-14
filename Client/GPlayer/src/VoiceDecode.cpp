#include "VoiceDecode.h"
#include "GtooLogger.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
// #include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}

VoiceDecode::VoiceDecode() {
}

VoiceDecode::~VoiceDecode() {
}

#define MAX_AUDIO_FRAME_SIZE 19200

AVCodecContext *pCodecCtx = nullptr;
AVPacket *packet = nullptr;
AVFrame *pFrame = nullptr;
uint8_t *out_buffer;
struct SwrContext *auConvertCtx;

static uint8_t *audio_chunk;
static uint32_t audio_len;
static uint8_t *audio_pos;

// typedef void (SDLCALL * SDL_AudioCallback) (void *userdata, Uint8 * stream,
//     int len);
void read_audio_data(void *udata, Uint8 *stream, int len) {
    fprintf(stderr, "stream addr:%p, audio_len:%d, len:%d\n", stream, audio_len, len);
    // 首先使用SDL_memset()将stream中的数据设置为0
    SDL_memset(stream, 0, len);
    if (audio_len == 0) return;
    len = (len > audio_len ? audio_len : len);

    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    audio_pos += len;
    audio_len -= len;
}

bool VoiceDecode::open(const QString &url) {
    // QString
    // newUrl("E:\\Desktop\\languguetest\\Cplusplustest\\3-VisualStudio2017\\0-GtooPlayer\\test_video\\123.mp3");
    if (url.isNull()) return false;

    AVDictionary *dict = nullptr;
    av_dict_set(&dict, "rtsp_transport", "tcp",
                0);  // 设置rtsp流使用tcp打开，如果打开失败错误信息为【Error number -135
                     // occurred】可以切换（UDP、tcp、udp_multicast、http），比如vlc推流就需要使用udp打开
    av_dict_set(
        &dict, "max_delay", "3",
        0);  // 设置最大复用或解复用延迟（以微秒为单位）。当通过【UDP】
             // 接收数据时，解复用器尝试重新排序接收到的数据包（因为它们可能无序到达，或者数据包可能完全丢失）。这可以通过将最大解复用延迟设置为零（通过max_delayAVFormatContext
             // 字段）来禁用。
    av_dict_set(&dict, "timeout", "1000000",
                0);  // 以微秒为单位设置套接字 TCP I/O 超时，如果等待时间过短，也可能会还没连接就返回了。

    // 打开输入流并返回解封装上下文
    int ret = avformat_open_input(&pFormatCtx, url.toStdString().data(), nullptr, &dict);
    if (ret < 0) {
        LOG_ERR("Failed to open video file! avformat_open_input {:d}\n", ret);
        return false;
    }
    if (dict)  // 释放参数字典
    {
        av_dict_free(&dict);
    }

    ret = avformat_find_stream_info(pFormatCtx, nullptr);  // 读取媒体文件的数据包以获取流信息
    // 可以不调用
    // 获取播放时长的信息
    qint64 totalTime = pFormatCtx->duration / (AV_TIME_BASE / 1000);  // 计算视频总时长（毫秒）

    mVoiceIndex = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (mVoiceIndex < 0) {
        LOG_ERR("Din't find a video stream! av_find_best_stream {:d}\n", mVoiceIndex);
        return false;
    }

    pCodecPara = pFormatCtx->streams[mVoiceIndex]->codecpar;

    const AVCodec *pCodec = avcodec_find_decoder(pCodecPara->codec_id);
    if (pCodec == nullptr) {
        LOG_ERR("Unsupported codec! avcodec_find_decoder {:d}\n", mVoiceIndex);
        return false;
    }

    pCodecCtx = avcodec_alloc_context3(pCodec);
    ret = avcodec_parameters_to_context(pCodecCtx, pCodecPara);
    if (ret < 0) {
        LOG_ERR("Couldn't copy codec context! avcodec_parameters_to_context {:d}\n", ret);
        return false;
    }
    ret = avcodec_open2(pCodecCtx, pCodec, nullptr);
    if (ret < 0) {
        LOG_ERR("Failed to open decoder! avcodec_open2 {:d}\n", ret);
        return false;
    }
    packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    av_init_packet(packet);
    pFrame = av_frame_alloc();

    uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;       // 输出立体声

    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;  // 输出格式S16

    int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
    int out_sample_rate = 44100;
    int out_nb_samples = 1024;

    int out_buffer_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);
    out_buffer = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);

    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        LOG_ERR("Could not initialize SDL {:s}\n", SDL_GetError());
        return false;
    }

    SDL_AudioSpec spec;
    spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = out_channels;
    spec.silence = 0;
    spec.samples = 1024;
    spec.callback = read_audio_data;
    spec.userdata = pCodecCtx;

    // SDL_AudioSpec spec;
    // spec.freq = 44100;//根据你录制的PCM采样率决定
    // spec.format = AUDIO_S16SYS;
    // spec.channels = 1; //单声道
    // spec.silence = 0;
    // spec.samples = 1024;
    // spec.callback = read_audio_data2;
    // spec.userdata = NULL;

    int64_t in_channel_layout;
    in_channel_layout = av_get_default_channel_layout(pCodecCtx->channels);
    printf("in_channel_layout --->%d\n", in_channel_layout);
    auConvertCtx = swr_alloc();
    auConvertCtx = swr_alloc_set_opts(auConvertCtx, out_channel_layout, out_sample_fmt, out_sample_rate,
                                      in_channel_layout, pCodecCtx->sample_fmt, pCodecCtx->sample_rate, 0, NULL);
    swr_init(auConvertCtx);

    if (SDL_OpenAudio(&spec, NULL) < 0) {
        printf("can't open audio.\n");
        return false;
    }

    SDL_PauseAudio(0);
    while (av_read_frame(pFormatCtx, packet) >= 0) {
        if (packet->stream_index == mVoiceIndex) {
            avcodec_send_packet(pCodecCtx, packet);
            while (avcodec_receive_frame(pCodecCtx, pFrame) == 0) {
                swr_convert(auConvertCtx, &out_buffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)pFrame->data,
                            pFrame->nb_samples);  // 转换音频
            }

            audio_chunk = (Uint8 *)out_buffer;
            audio_len = out_buffer_size;
            audio_pos = audio_chunk;
            LOG_DBG("audio_len:{:d}", audio_len);
            while (audio_len > 0) {
                SDL_Delay(1);  // 延迟播放
            }
        }
        av_packet_unref(packet);
    }
    swr_free(&auConvertCtx);
    SDL_Quit();
}