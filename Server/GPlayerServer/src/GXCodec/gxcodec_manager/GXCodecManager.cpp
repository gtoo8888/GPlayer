#include "GXCodecManager.h"

GXCodecManager::GXCodecManager() {
}

void GXCodecManager::pushMedia(MediaProfiler* mediaProfiler) {
    mpTranscodingTask = std::make_shared<GXCodecTask>(mediaProfiler);
}

void GXCodecManager::run() {
    // mpTranscodingTask->run();
    mpTranscodingTask->splitAudioStream();
}
