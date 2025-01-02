#pragma once
#include <QString>
#include "global.h"

class GUtils {
public:
    static QString unix2StrTimeQString(int64 time);
    static std::string unix2StrTimeChrono(int64 time);
    static int64 str2Unix(const std::string& str_time);

    static std::string getFilename(const std::string& path);
    static std::string getFilenameWithoutExe(const std::string& path);
    static std::string getFileSuffix(const std::string& path);
    static std::string getAbsolutePath(const std::string& path);
    static bool splitStringAtDelimiter(const std::string& str, char delimiter, std::string& left, std::string& right);

    static bool isLetterC(char letter);
    static bool isLetterCplusplus(char letter);
};
