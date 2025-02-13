#ifndef TRANSCODINGMANAGER_H
#define TRANSCODINGMANAGER_H

#include "global.h"

class TranscodingTask;

class TranscodingManager {
public:
    TranscodingManager();
    ~TranscodingManager() = default;

    void run();

    std::priority_queue<int> sprmTranscodingTask;
};

#endif  // TRANSCODINGMANAGER_H