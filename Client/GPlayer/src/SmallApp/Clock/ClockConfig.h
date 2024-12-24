#ifndef CLOCKCONFIG_H
#define CLOCKCONFIG_H

#include <QWidget>
#include <vector>
#include "cJSON.h"

bool parseJsonBool(cJSON* root, const char* jsonName);

class Config {
public:
    Config() = default;
    virtual ~Config() = default;

    // 父类没有实现，需要定义为纯虚函数
    virtual void loadConfig(cJSON* root) = 0;
    virtual void printConfig() = 0;
};

class Setting : public Config {
public:
    Setting();
    virtual ~Setting() override {};
    virtual void loadConfig(cJSON* root) override;
    virtual void printConfig() override;

    enum DisplayUnit {
        sec,
        msec
    };

    enum Languague {
        Chinese,
        English
    };

    uint8_t StopwatchCount;
    DisplayUnit displayUnit;
    bool bDrag;
    bool bSlowFastDisplay;
    bool bAUtoClean;
    uint16_t autoCleanTime;
    Languague languague;
};

//"keyshot": {
//  "Pause": "Space",
//  "CoutTime": "ctrl+D",
//  "Close": "Esc",
//  "NewClock": "N"
//},

class KeyShot : public Config {
public:
    enum Key {
        none,
        space,
        ctrl,
        alt,
        esc,
        d,
        n,
    };

    KeyShot();
    virtual ~KeyShot() override {};
    virtual void loadConfig(cJSON* root) override;
    virtual void printConfig() override;
    Key keyshortCharToEnum(char* keyshort);
    std::string keyshortEnumToString(Key keyshort);

    std::vector<Key> ePause;
    std::vector<Key> eCoutTime;
    std::vector<Key> eClose;
    std::vector<Key> eNewClock;
};

class ClockAbout : public Config {
public:
    ClockAbout();
    virtual ~ClockAbout() override {};
    virtual void loadConfig(cJSON* root) override;
    virtual void printConfig() override;

    std::string appName;
    std::string appVersion;
};

// 未来可以使用ConfigInface替代ClockConfig，作为通用的配置接口类
class ConfigInface {
    ConfigInface();
    ~ConfigInface();

public:
    Setting* settingConfig;
    KeyShot* keyshotConfig;
    ClockAbout* aboutConfig;
};

class ClockConfig {
public:
    ClockConfig();
    ClockConfig(QString configPath);
    ~ClockConfig();

    Setting* settingConfig;
    KeyShot* keyshotConfig;
    ClockAbout* aboutConfig;
};

#endif  // CLOCKCONFIG_H
