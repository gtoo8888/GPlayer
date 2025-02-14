#include "global.h"

GXCodecLogger* GXCodecLogger::instanceLogger = nullptr;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        LOG_ERR("need video file:\r\n");
        return -1;
    }

    LOG_INF("GXCodecTask {} start", Version::getVersionStr());
    std::string inputFileName = std::string(argv[1]);
    MediaProfiler* mediaProfiler = new MediaProfiler(std::string(argv[1]));

    std::shared_ptr<GXCodecManager> mGXCodecManager = std::make_shared<GXCodecManager>();
    mGXCodecManager->pushMedia(mediaProfiler);
    mGXCodecManager->run();
    LOG_INF("GXCodecTask {} finish", Version::getVersionStr());

    return 0;
}