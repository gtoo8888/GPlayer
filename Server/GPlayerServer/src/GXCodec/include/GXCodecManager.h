#ifndef GXCODECMANAGER_H
#define GXCODECMANAGER_H

#include "global.h"

class GXCodecTask;
class MediaProfiler;

class GXCodecManager {
public:
    GXCodecManager();
    ~GXCodecManager() = default;

    void pushMedia(MediaProfiler* mediaProfiler);
    void run();

    std::shared_ptr<GXCodecTask> mpTranscodingTask;
    // std::priority_queue<int> sprmTranscodingTask;
};

#endif  // GXCODECMANAGER_H