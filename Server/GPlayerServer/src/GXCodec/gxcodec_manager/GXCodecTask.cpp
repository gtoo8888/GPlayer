#include "GXCodecTask.h"

GXCodecTask::GXCodecTask() {
}

GXCodecTask::GXCodecTask(MediaProfiler *mediaProfiler) {
    mpMediaProfiler = mediaProfiler;
}

bool GXCodecTask::run(void) {
    if (!openFile()) {
        return false;
    }
    if (!showFormatInfo()) {
        return false;
    }
    if (!getCoder()) {
        return false;
    }
    if (!free()) {
        return false;
    }
    return true;
}

bool GXCodecTask::splitAudioStream(void) {
    if (!openFile()) {
        return false;
    }
    if (!showFormatInfo()) {
        return false;
    }
    if (!getCoder()) {
        return false;
    }
    // if (!saveAudioFile()) {
    //     return false;
    // }
    if (!saveVideoFile()) {
        return false;
    }
    return true;
}

bool GXCodecTask::openFile(void) {
    if (avformat_open_input(&mpFormatCtx, mpMediaProfiler->mInputFilename.c_str(), nullptr, nullptr) != 0) {
        LOG_ERR("cant open file");
        return false;
    }
    LOG_DBG("avformat_open_input {:s} success", mpMediaProfiler->mInputFilename);

    if (avformat_find_stream_info(mpFormatCtx, nullptr) < 0) {
        LOG_ERR("cant find stream info");
        return false;
    }
    LOG_DBG("avformat_find_stream_info");
    return true;
}

bool GXCodecTask::showFormatInfo() {
    av_dump_format(mpFormatCtx, 0, mpMediaProfiler->mInputFilename.c_str(), 0);

    LOG_DBG("Number of streams: {}", mpFormatCtx->nb_streams);
    LOG_DBG("Duration: {} microseconds", mpFormatCtx->duration);
    LOG_DBG("Bit rate: {} bit/s", mpFormatCtx->bit_rate);

    showStreamInfo(mpFormatCtx);

    AVDictionaryEntry *tag = nullptr;
    while ((tag = av_dict_get(mpFormatCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        LOG_DBG("Metadata: {} = {}", tag->key, tag->value);
    }

    return true;
}

bool GXCodecTask::showStreamInfo(AVFormatContext *formatCtx) {
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

bool GXCodecTask::getCoder(void) {
    for (unsigned int i = 0; i < mpFormatCtx->nb_streams; i++) {
        if (mpFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            mpCodecParams = mpFormatCtx->streams[i]->codecpar;
            break;
        }
    }
    if (videoStream == -1) {
        LOG_ERR("cant find video stream");
        return false;
    }

    const AVCodec *mpCodec = avcodec_find_decoder(mpCodecParams->codec_id);
    if (!mpCodec) {
        LOG_ERR("dont support decoder!");
        return false;
    }
    LOG_DBG("find decoder success");

    AVCodecContext *pCodecCtx = avcodec_alloc_context3(mpCodec);
    if (!pCodecCtx) {
        LOG_ERR("alloce code context");
        return false;
    }
    if (avcodec_parameters_to_context(pCodecCtx, mpCodecParams) < 0) {
        LOG_ERR("copy codec para fail");
        return false;
    }
    LOG_DBG("avcodec_alloc_context3n");

    if (avcodec_open2(pCodecCtx, mpCodec, nullptr) < 0) {
        LOG_ERR("cant open code");
        return false;
    }
    LOG_DBG("avcodec_open2");

    avcodec_free_context(&pCodecCtx);
    return true;
}

bool GXCodecTask::saveVideoFile(void) {
    char errbuf[AV_ERROR_MAX_STRING_SIZE];
    int audio_stream_idx = -1;
    for (int i = 0; i < mpFormatCtx->nb_streams; i++) {
        if (mpFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            audio_stream_idx = i;
            break;
        }
    }

    std::string output_file = "test.mp4";
    AVFormatContext *ouputFormatCtx = nullptr;

    // std::string output_file = "test";
    // const AVFormatContext *ouputFormatCtx = av_guess_format("mp3", nullptr, nullptr);
    avformat_alloc_output_context2(&ouputFormatCtx, nullptr, nullptr, output_file.c_str());

    AVStream *out_stream = avformat_new_stream(ouputFormatCtx, nullptr);
    int ret = avcodec_parameters_copy(out_stream->codecpar, mpFormatCtx->streams[audio_stream_idx]->codecpar);
    if (ret < 0) {
        av_strerror(ret, errbuf, sizeof(errbuf));
        LOG_ERR("Failed to copy codec parameters: %s\n", errbuf);
        return false;
    }
    out_stream->codecpar->codec_tag = 0;

    if (!ouputFormatCtx->oformat->audio_codec) {
        LOG_ERR("Output format does not support audio streams.\n");
        return false;
    }
    out_stream->codecpar->codec_id = ouputFormatCtx->oformat->audio_codec;
    out_stream->codecpar->sample_rate = 44100;  // 设置采样率
    // out_stream->codecpar->channel_layout = AV_CH_LAYOUT_STEREO;  // 设置声道布局
    // out_stream->codecpar->channels = 2;                          // 设置声道数

    avio_open(&ouputFormatCtx->pb, output_file.c_str(), AVIO_FLAG_WRITE);
    avformat_write_header(ouputFormatCtx, nullptr);

    AVPacket pkt;
    av_init_packet(&pkt);
    while (av_read_frame(mpFormatCtx, &pkt) >= 0) {
        // // 打印输入文件的流信息
        // showStreamInfo(mpFormatCtx);

        // // 打印输出文件的流信息
        // showStreamInfo(ouputFormatCtx);

        if (pkt.stream_index == audio_stream_idx) {
            // 调整时间戳
            pkt.pts = av_rescale_q_rnd(pkt.pts, mpFormatCtx->streams[audio_stream_idx]->time_base,
                                       out_stream->time_base, AV_ROUND_NEAR_INF);
            pkt.dts = pkt.pts;
            pkt.duration =
                av_rescale_q(pkt.duration, mpFormatCtx->streams[audio_stream_idx]->time_base, out_stream->time_base);
            pkt.pos = -1;

            int ret = av_interleaved_write_frame(ouputFormatCtx, &pkt);
            if (ret < 0) {
                av_strerror(ret, errbuf, sizeof(errbuf));
                fprintf(stderr, "Error writing audio frame: %s\n", errbuf);
                av_packet_unref(&pkt);
                break;
            }
        }
        av_packet_unref(&pkt);
    }

    av_write_trailer(ouputFormatCtx);
    avio_closep(&ouputFormatCtx->pb);
    avformat_free_context(ouputFormatCtx);
    avformat_close_input(&mpFormatCtx);
    return true;
}

bool GXCodecTask::saveAudioFile(void) {
    char errbuf[AV_ERROR_MAX_STRING_SIZE];
    int audio_stream_idx = -1;
    for (int i = 0; i < mpFormatCtx->nb_streams; i++) {
        if (mpFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_idx = i;
            break;
        }
    }

    std::string output_file = "test.aac";
    AVFormatContext *ouputFormatCtx = nullptr;

    // std::string output_file = "test";
    // const AVFormatContext *ouputFormatCtx = av_guess_format("mp3", nullptr, nullptr);
    avformat_alloc_output_context2(&ouputFormatCtx, nullptr, nullptr, output_file.c_str());

    AVStream *out_stream = avformat_new_stream(ouputFormatCtx, nullptr);
    int ret = avcodec_parameters_copy(out_stream->codecpar, mpFormatCtx->streams[audio_stream_idx]->codecpar);
    if (ret < 0) {
        av_strerror(ret, errbuf, sizeof(errbuf));
        LOG_ERR("Failed to copy codec parameters: %s\n", errbuf);
        return false;
    }
    out_stream->codecpar->codec_tag = 0;

    if (!ouputFormatCtx->oformat->audio_codec) {
        LOG_ERR("Output format does not support audio streams.\n");
        return false;
    }
    out_stream->codecpar->codec_id = ouputFormatCtx->oformat->audio_codec;
    out_stream->codecpar->sample_rate = 44100;  // 设置采样率
    // out_stream->codecpar->channel_layout = AV_CH_LAYOUT_STEREO;  // 设置声道布局
    // out_stream->codecpar->channels = 2;                          // 设置声道数

    avio_open(&ouputFormatCtx->pb, output_file.c_str(), AVIO_FLAG_WRITE);
    avformat_write_header(ouputFormatCtx, nullptr);

    AVPacket pkt;
    av_init_packet(&pkt);
    while (av_read_frame(mpFormatCtx, &pkt) >= 0) {
        // 打印输入文件的流信息
        showStreamInfo(mpFormatCtx);

        // 打印输出文件的流信息
        showStreamInfo(ouputFormatCtx);

        if (pkt.stream_index == audio_stream_idx) {
            // 调整时间戳
            pkt.pts = av_rescale_q_rnd(pkt.pts, mpFormatCtx->streams[audio_stream_idx]->time_base,
                                       out_stream->time_base, AV_ROUND_NEAR_INF);
            pkt.dts = pkt.pts;
            pkt.duration =
                av_rescale_q(pkt.duration, mpFormatCtx->streams[audio_stream_idx]->time_base, out_stream->time_base);
            pkt.pos = -1;

            int ret = av_interleaved_write_frame(ouputFormatCtx, &pkt);
            if (ret < 0) {
                av_strerror(ret, errbuf, sizeof(errbuf));
                fprintf(stderr, "Error writing audio frame: %s\n", errbuf);
                av_packet_unref(&pkt);
                break;
            }
        }
        av_packet_unref(&pkt);
    }

    av_write_trailer(ouputFormatCtx);
    avio_closep(&ouputFormatCtx->pb);
    avformat_free_context(ouputFormatCtx);
    avformat_close_input(&mpFormatCtx);
    return true;
}

bool GXCodecTask::free(void) {
    avformat_close_input(&mpFormatCtx);
    return false;
}
