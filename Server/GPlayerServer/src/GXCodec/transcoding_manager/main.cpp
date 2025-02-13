#include "global.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("need video file:\r\n");
        return -1;
    }

    std::string inputFileName = std::string(argv[1]);
    MediaProfiler mediaProfiler(std::string(argv[1]));

    TranscodingManager *transcodingManager(MediaProfiler);

    AVFormatContext *pFormatCtx = nullptr;

    if (avformat_open_input(&pFormatCtx, inputFileName.c_str(), NULL, NULL) != 0) {
        fprintf(stderr, "cant open file\n");
        return -1;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        fprintf(stderr, "cant find stream info\n");
        return -1;
    }
    av_dump_format(pFormatCtx, 0, inputFileName.c_str(), 0);

    int videoStream = -1;
    AVCodecParameters *pCodecParams = nullptr;
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            pCodecParams = pFormatCtx->streams[i]->codecpar;
            break;
        }
    }
    if (videoStream == -1) {
        fprintf(stderr, "cant find video stream\n");
        return -1;
    }

    const AVCodec *pCodec = avcodec_find_decoder(pCodecParams->codec_id);
    if (!pCodec) {
        fprintf(stderr, "dont support decoder!\n");
        return -1;
    }

    AVCodecContext *pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx) {
        fprintf(stderr, "alloce code context\n");
        return -1;
    }
    if (avcodec_parameters_to_context(pCodecCtx, pCodecParams) < 0) {
        fprintf(stderr, "copy codec para fail\n");
        return -1;
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        fprintf(stderr, "cant open code\n");
        return -1;
    }

    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}