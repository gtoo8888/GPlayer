#ifndef VOICEDECODE_H
#define VOICEDECODE_H

#include <QString>
#include "SDL.h"

struct AVFormatContext;
struct AVCodecParameters;

extern void read_audio_data(void* udata, Uint8* stream, int len);

class VoiceDecode {
public:
    VoiceDecode();
    ~VoiceDecode();

    bool open(const QString& m_url);

private:
    AVFormatContext* pFormatCtx = nullptr;
    AVCodecParameters* pCodecPara = nullptr;
    int mVoiceIndex = 0;
};

#endif
