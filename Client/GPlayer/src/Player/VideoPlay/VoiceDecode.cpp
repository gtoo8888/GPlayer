#include "VoiceDecode.h"

#define MAX_AUDIO_FRAME_SIZE 19200

static uint8_t *audio_chunk;
static uint32_t audio_len;
static uint8_t *audio_pos;

// typedef void (SDLCALL * SDL_AudioCallback) (void *userdata, Uint8 * stream, int len);
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

bool VoiceDecode::showFormatInfo(AVFormatContext *formatCtx) {
    LOG_DBG("Number of streams: {}", formatCtx->nb_streams);
    LOG_DBG("Duration: {} microseconds", formatCtx->duration);
    LOG_DBG("Bit rate: {} bit/s", formatCtx->bit_rate);

    showStreamInfo(formatCtx);

    AVDictionaryEntry *tag = nullptr;
    while ((tag = av_dict_get(formatCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        LOG_DBG("Metadata: {} = {}", tag->key, tag->value);
    }

    return true;
}

bool VoiceDecode::showStreamInfo(AVFormatContext *formatCtx) {
    for (uint8 idx = 0; idx < formatCtx->nb_streams; idx++) {
        AVStream *stream = formatCtx->streams[idx];
        AVCodecParameters *codecpar = formatCtx->streams[idx]->codecpar;
        LOG_DBG("Stream #{}: codec type = {}", idx, av_get_media_type_string(stream->codecpar->codec_type));
        LOG_DBG("Stream #{}: codec ID = {}", idx, static_cast<int>(stream->codecpar->codec_id));
        LOG_DBG("Stream #{}: bit rate = {}", idx, stream->codecpar->bit_rate);
        LOG_DBG("Stream #{}: width = {}, height = {}", idx, stream->codecpar->width, stream->codecpar->height);
    }
    return true;
}

bool VoiceDecode::openVoiceUrl(const std::string &voiceUrl) {
    if (voiceUrl.empty()) {
        return false;
    }
    nowVoiceUrl = voiceUrl;
    LOG_DBG("{:s}\n", nowVoiceUrl);

    if (!initFormat()) {
        return false;
    }
    if (!initCodec()) {
        return false;
    }
    if (!initSDLPara()) {
        return false;
    }

    int out_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);  // 输出立体声

    int out_sample_rate = 44100;
    int out_nb_samples = 1024;
    int input_sample_rate = pCodecCtx->sample_rate;

    out_buffer = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);

    int64_t in_channel_layout = av_get_default_channel_layout(pCodecCtx->channels);
    printf("in_channel_layout --->%lld\n", in_channel_layout);
    auConvertCtx = swr_alloc();
    auConvertCtx = swr_alloc_set_opts(auConvertCtx, av_get_default_channel_layout(pCodecCtx->channels),
                                      AV_SAMPLE_FMT_S16, input_sample_rate, in_channel_layout, pCodecCtx->sample_fmt,
                                      pCodecCtx->sample_rate, 0, NULL);
    swr_init(auConvertCtx);

    if (!playVoice()) {
        return false;
    }

    swr_free(&auConvertCtx);
    SDL_Quit();
    return true;
}

bool VoiceDecode::initFormat(void) {
    AVDictionary *dict = nullptr;
    av_dict_set(&dict, "rtsp_transport", "tcp", 0);
    av_dict_set(&dict, "max_delay", "3", 0);
    av_dict_set(&dict, "timeout", "1000000", 0);

    // 打开输入流并返回解封装上下文
    int ret = -1;
    ret = avformat_open_input(&pFormatCtx, nowVoiceUrl.c_str(), nullptr, &dict);
    if (ret < 0) {
        LOG_ERR("Failed to open video file! avformat_open_input {:d}\n", ret);
        return false;
    }
    if (dict) {
        av_dict_free(&dict);
    }

    ret = avformat_find_stream_info(pFormatCtx, nullptr);  // 读取媒体文件的数据包以获取流信息
    if (ret < 0) {
        LOG_ERR("Failed to get stream info: {}", ret);
        avformat_close_input(&pFormatCtx);
        return false;
    }

    av_dump_format(pFormatCtx, 0, nowVoiceUrl.c_str(), 0);
    showFormatInfo(pFormatCtx);

    // 获取播放时长的信息
    qint64 totalTime = pFormatCtx->duration / (AV_TIME_BASE / 1000);  // 计算视频总时长（毫秒）

    mVoiceIndex = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (mVoiceIndex < 0) {
        LOG_ERR("Don't find a video stream! av_find_best_stream {:d}\n", mVoiceIndex);
        avformat_close_input(&pFormatCtx);
        return false;
    }

    return true;
}

bool VoiceDecode::initCodec(void) {
    int ret = -1;
    pCodecPara = pFormatCtx->streams[mVoiceIndex]->codecpar;

    const AVCodec *pCodec = avcodec_find_decoder(pCodecPara->codec_id);
    if (pCodec == nullptr) {
        LOG_ERR("Unsupported codec! avcodec_find_decoder {:d}\n", mVoiceIndex);
        return false;
    }

    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx) {
        LOG_ERR("Unsupported codec");
        avformat_close_input(&pFormatCtx);
        return false;
    }
    ret = avcodec_parameters_to_context(pCodecCtx, pCodecPara);
    if (ret < 0) {
        LOG_ERR("Couldn't copy codec context! avcodec_parameters_to_context {:d}\n", ret);
        avcodec_free_context(&pCodecCtx);
        avformat_close_input(&pFormatCtx);
        return false;
    }
    ret = avcodec_open2(pCodecCtx, pCodec, nullptr);
    if (ret < 0) {
        LOG_ERR("Failed to open decoder! avcodec_open2 {:d}\n", ret);
        avcodec_free_context(&pCodecCtx);
        avformat_close_input(&pFormatCtx);
        return false;
    }

    return true;
}

bool VoiceDecode::initSDLPara(void) {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        LOG_ERR("SDL initialization failed: {}", SDL_GetError());
        avcodec_free_context(&pCodecCtx);
        avformat_close_input(&pFormatCtx);
        return false;
    }

    // 提取输入音频参数
    int input_sample_rate = pCodecCtx->sample_rate;
    int input_channels = pCodecCtx->channels;
    AVSampleFormat input_sample_fmt = static_cast<AVSampleFormat>(pCodecCtx->sample_fmt);

    // 配置 SDL_AudioSpec
    SDL_AudioSpec desiredSpec;
    desiredSpec.freq = input_sample_rate;  // 根据你录制的PCM采样率决定
    desiredSpec.format = AUDIO_S16SYS;     // SDL 常用格式，必要时可在回调中转换
    desiredSpec.channels = input_channels;
    desiredSpec.silence = 0;
    desiredSpec.samples = 1024;
    desiredSpec.callback = read_audio_data;
    desiredSpec.userdata = pCodecCtx;  // 传递当前对象指针以便访问成员变量

    if (SDL_OpenAudio(&desiredSpec, NULL) < 0) {
        LOG_ERR("Failed to open audio device: {}", SDL_GetError());
        SDL_Quit();
        avcodec_free_context(&pCodecCtx);
        avformat_close_input(&pFormatCtx);
        return false;
    }

    return true;
}

bool VoiceDecode::playVoice(void) {
    int out_buffer_size = av_samples_get_buffer_size(NULL, av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO), 1024,
                                                     AV_SAMPLE_FMT_S16, 1);

    SDL_PauseAudio(0);
    packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    av_init_packet(packet);
    pFrame = av_frame_alloc();
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

    return true;
}
