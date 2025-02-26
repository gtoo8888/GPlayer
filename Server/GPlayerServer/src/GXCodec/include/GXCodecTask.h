#ifndef GXCODECTASK_H
#define GXCODECTASK_H

#include "global.h"

class MediaProfiler;

class GXCodecTask {
public:
    GXCodecTask();
    GXCodecTask(MediaProfiler* mediaProfiler);
    ~GXCodecTask() = default;

    bool run(void);
    bool splitAudioStream(void);

    bool openFile(void);
    bool showFormatInfo(void);
    bool showStreamInfo(AVFormatContext* formatCtx) ;
    bool getCoder(void);
    bool saveAudioFile(void);
    bool saveVideoFile(void);
    bool free(void);

    MediaProfiler* mpMediaProfiler = nullptr;
    std::string mInputFileName;

    AVFormatContext* mpFormatCtx = nullptr;
    int32 videoStream = -1;
    AVCodecParameters* mpCodecParams = nullptr;
};

#endif  // GXCODECTASK_H