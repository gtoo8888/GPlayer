#ifndef FFMPEGDECODER_H
#define FFMPEGDECODER_H

#include "global.h"

class FFmpegDecoder {
public:
    FFmpegDecoder();
    ~FFmpegDecoder() = default;
};

class VideoDecoder : public FFmpegDecoder {
public:
    VideoDecoder();
    ~VideoDecoder() = default;
};

#endif  // FFMPEGDECODER_H