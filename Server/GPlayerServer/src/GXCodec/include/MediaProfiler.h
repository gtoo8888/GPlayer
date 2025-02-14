#ifndef MEDIAPROFILER_H
#define MEDIAPROFILER_H

#include "global.h"

class MediaProfiler {
public:
    MediaProfiler();
    MediaProfiler(std::string filePath);
    ~MediaProfiler() = default;

    std::string mInputFilename;
};

#endif  // MEDIAPROFILER_H