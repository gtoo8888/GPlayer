#ifndef VOICEDECODE_H
#define VOICEDECODE_H

#include "global.h"

struct AVFormatContext;
struct AVCodecParameters;

extern void read_audio_data(void *udata, Uint8 *stream, int len);

class VoiceDecode {
public:
    VoiceDecode() = default;
    ~VoiceDecode() = default;

    bool openVoiceUrl(const std::string &url);
    bool initFormat(void);
    bool initCodec(void);
    bool initSDLPara(void);
    bool playVoice(void);
    bool showFormatInfo(AVFormatContext *formatCtx);
    bool showStreamInfo(AVFormatContext *formatCtx);

private:
    AVFormatContext *pFormatCtx = nullptr;
    AVCodecParameters *pCodecPara = nullptr;
    AVCodecContext *pCodecCtx = nullptr;
    AVPacket *packet = nullptr;
    AVFrame *pFrame = nullptr;
    uint8 *out_buffer;
    SwrContext *auConvertCtx;
    std::string nowVoiceUrl;
    int mVoiceIndex = 0;
};

#endif
