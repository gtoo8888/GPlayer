#ifndef VOICEDECODE_H
#define VOICEDECODE_H

#include <QString>
#include "SDL.h"

struct AVFormatContext;
struct AVCodecParameters;

extern void read_audio_data(void* udata, Uint8* stream, int len);

class VoiceDecode {
public:
    VoiceDecode() = default;
    ~VoiceDecode() = default;

    bool open(const QString& mUrl);

private:
    AVFormatContext* pFormatCtx = nullptr;
    AVCodecParameters* pCodecPara = nullptr;
    int mVoiceIndex = 0;
};

#endif
