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
    if (!getStreamInfo()) {
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
    if (!getStreamInfo()) {
        return false;
    }
    return true;
}

bool GXCodecTask::openFile(void) {
    if (avformat_open_input(&mpFormatCtx, mpMediaProfiler->mInputFilename.c_str(), NULL, NULL) != 0) {
        LOG_ERR("cant open file");
        return false;
    }
    LOG_DBG("avformat_open_input {:s} success", mpMediaProfiler->mInputFilename);

    if (avformat_find_stream_info(mpFormatCtx, NULL) < 0) {
        LOG_ERR("cant find stream info");
        return false;
    }
    LOG_DBG("avformat_find_stream_info");
    return true;
}

bool GXCodecTask::getStreamInfo() {
    // av_dump_format(mpFormatCtx, 0, mpMediaProfiler->mInputFilename.c_str(), 0);

    LOG_DBG("Number of streams: {}", mpFormatCtx->nb_streams);
    LOG_DBG("Duration: {} microseconds", mpFormatCtx->duration);
    LOG_DBG("Bit rate: {} bit/s", mpFormatCtx->bit_rate);

    for (unsigned int idx = 0; idx < mpFormatCtx->nb_streams; idx++) {
        AVStream *stream = mpFormatCtx->streams[idx];
        AVCodecParameters *codecpar = mpFormatCtx->streams[idx]->codecpar;
        LOG_DBG("Stream #{}: codec type = {}", idx, av_get_media_type_string(stream->codecpar->codec_type));
        LOG_DBG("Stream #{}: codec ID = {}", idx, static_cast<int>(stream->codecpar->codec_id));
        LOG_DBG("Stream #{}: bit rate = {}", idx, stream->codecpar->bit_rate);
        LOG_DBG("Stream #{}: width = {}, height = {}", idx, stream->codecpar->width, stream->codecpar->height);
    }

    AVDictionaryEntry *tag = NULL;
    while ((tag = av_dict_get(mpFormatCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        LOG_DBG("Metadata: {} = {}", tag->key, tag->value);
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

    if (avcodec_open2(pCodecCtx, mpCodec, NULL) < 0) {
        LOG_ERR("cant open code");
        return false;
    }
    LOG_DBG("avcodec_open2");

    avcodec_free_context(&pCodecCtx);
    return false;
}

bool GXCodecTask::free(void) {
    avformat_close_input(&mpFormatCtx);
    return false;
}
