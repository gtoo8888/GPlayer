#ifndef VERSION_H
#define VERSION_H

#include "global.h"

class Version {
public:
    static const uint8 major = 0;
    static const uint8 minor = 0;
    static const uint8 patch = 3;

    static std::string getVersionStr(void) {
        char version[15];
        sprintf(version, "v%i.%i.%i", major, minor, patch);
        return std::string(version);
    }
};

#endif  // VERSION_H
