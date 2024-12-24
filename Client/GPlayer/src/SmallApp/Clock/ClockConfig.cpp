#include "ClockConfig.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>     // [ ]
#include <QJsonDocument>  // 解析Json
#include <QJsonObject>    // { }
#include <QJsonParseError>
#include <QJsonValue>     // int float double bool null { } [ ]
#include "GtooPlayer.h"

ClockConfig::ClockConfig() {
}

// ClockConfig::ClockConfig(QString configPath) {
//     QFile file(configPath);
//     QByteArray jsonData;
//     LOG_INF("ClockConfig::ClockConfig");
//     if (file.open(QIODevice::ReadOnly)) {
//         jsonData = file.readAll();
//         file.close();
//     }else{
//         LOG_ERR("file.open fail");
//     }

//    QJsonDocument jsonDocu = QJsonDocument::fromJson(jsonData);

//    if (jsonDocu.isObject()) {
//        QJsonObject obj_root = jsonDocu.object();
//        QStringList keys = obj_root.keys();
//        for (auto key : keys) {
//            QJsonValue value = obj_root.value(key);
//            if (value.isString()) {
//                qDebug() << key << ": " << value.toString();
//            } else if (value.isDouble()) {
//                qDebug() << key << ": " << value.toInt();
//            } else if (value.isArray()) {
//                QJsonArray arr = value.toArray();
//                for (int i = 0; i < arr.count(); ++i) {
//                    if (arr.at(i).isString()) {
//                        qDebug() << key << ": " << arr.at(i).toString();
//                    }
//                    // 这里就不判断是否为其它类型了，因为测试文件已知为字符串，要写也和上面类似，无限套娃
//                }
//            } else if (value.isObject()) {
//                QJsonObject subObj = value.toObject();
//                QStringList subKeys = subObj.keys();
//                for (auto subKey : subKeys) {
//                    QJsonValue subValue = subObj.value(subKey);
//                    if (subValue.isString()) {
//                        qDebug() << subKey << ": " << subValue.toString();
//                    }
//                    // 这里就不判断是否为其它类型了，因为测试文件已知为字符串，要写也和上面类似，无限套娃
//                }
//            }
//        }
//    }
//}

ClockConfig::ClockConfig(QString configPath) {
    QFile file(configPath);
    QByteArray jsonData;
    LOG_INF("ClockConfig::ClockConfig");
    if (file.open(QIODevice::ReadOnly)) {
        jsonData = file.readAll();
        file.close();
    } else {
        LOG_ERR("file.open fail");
    }

    settingConfig = new Setting();
    keyshotConfig = new KeyShot();
    aboutConfig = new ClockAbout();

    char* date = jsonData.data();
    cJSON* root = cJSON_Parse(date);
    cJSON* cjsonConfig = cJSON_GetObjectItem(root, "config");

    cJSON* cjsonSetting = cJSON_GetObjectItem(cjsonConfig, "setting");
    settingConfig->loadConfig(cjsonSetting);
    settingConfig->printConfig();

    cJSON* cjsonKeyshort = cJSON_GetObjectItem(cjsonConfig, "keyshot");
    keyshotConfig->loadConfig(cjsonKeyshort);
    keyshotConfig->printConfig();

    cJSON* cjsonAbout = cJSON_GetObjectItem(cjsonConfig, "about");
    aboutConfig->loadConfig(cjsonAbout);
    aboutConfig->printConfig();
}

Setting::Setting() {
}

void Setting::loadConfig(cJSON* root) {
    cJSON* cjsonStopwatchCount = cJSON_GetObjectItem(root, "StopwatchCount");
    cJSON* cjsonDisplayUnit = cJSON_GetObjectItem(root, "DisplayUnit");
    cJSON* cjsonBDrag = cJSON_GetObjectItem(root, "bDrag");
    cJSON* cjsonBSlowFastDisplay = cJSON_GetObjectItem(root, "bSlowFastDisplay");
    cJSON* cjsonBAutoClean = cJSON_GetObjectItem(root, "bAutoClean");
    cJSON* cjsonAutoCleanTime = cJSON_GetObjectItem(root, "autoCleanTime");
    cJSON* cjsonLanguage = cJSON_GetObjectItem(root, "Language");

    if (cjsonStopwatchCount->type == cJSON_Number) {
        StopwatchCount = static_cast<uint8_t>(cjsonStopwatchCount->valueint);
    } else {
        LOG_ERR("cjsonStopwatchCount type error");
    }

    if (cjsonDisplayUnit->type == cJSON_String) {
        if (strcmp(cjsonDisplayUnit->valuestring, "sec")) {
            displayUnit = Setting::DisplayUnit::sec;
        } else if (strcmp(cjsonDisplayUnit->valuestring, "msec")) {
            displayUnit = Setting::DisplayUnit::msec;
        }
    } else {
        LOG_ERR("cjsonDisplayUnit type error");
    }

    bDrag = parseJsonBool(cjsonBDrag, static_cast<const char*>("bDrag"));
    bSlowFastDisplay = parseJsonBool(cjsonBSlowFastDisplay, static_cast<const char*>("bSlowFastDisplay"));
    bAUtoClean = parseJsonBool(cjsonBAutoClean, static_cast<const char*>("bAutoClean"));

    if (cjsonAutoCleanTime->type == cJSON_Number) {
        autoCleanTime = cjsonAutoCleanTime->valueint;
    } else {
        LOG_ERR("cjsonAutoCleanTime type error");
    }

    if (cjsonLanguage->type == cJSON_String) {
        if (strcmp(cjsonLanguage->valuestring, "Chinese")) {
            languague = Languague::Chinese;
        } else if (strcmp(cjsonDisplayUnit->valuestring, "msec")) {
            languague = Languague::English;
        }
    } else {
        LOG_ERR("cjsonLanguage type error");
    }
}

void Setting::printConfig() {
    LOG_INF("Setting Config");
    LOG_INF("StopwatchCount: {}", StopwatchCount);
    switch (displayUnit) {
        case DisplayUnit::sec:
            LOG_INF("displayUnit: sec");
            break;
        case DisplayUnit::msec:
            LOG_INF("displayUnit: msec");
            break;
    };
    if (bDrag == true) {
        LOG_INF("bDrag: true");
    } else {
        LOG_INF("bDrag: false");
    }
    if (bSlowFastDisplay == true) {
        LOG_INF("bSlowFastDisplay: true");
    } else {
        LOG_INF("bSlowFastDisplay: false");
    }
    if (bAUtoClean == true) {
        LOG_INF("bAUtoClean: true");
    } else {
        LOG_INF("bAUtoClean: false");
    }
    LOG_INF("autoCleanTime: {}", autoCleanTime);
    switch (languague) {
        case Languague::Chinese:
            LOG_INF("languague: Chinese");
            break;
        case Languague::English:
            LOG_INF("languague: Chinese");
            break;
    };
    LOG_INF("");
}

KeyShot::KeyShot() {
}

void KeyShot::loadConfig(cJSON* root) {
    cJSON* cjsonPause = cJSON_GetObjectItem(root, "Pause");
    cJSON* cjsonCoutTime = cJSON_GetObjectItem(root, "CoutTime");
    cJSON* cjsonClose = cJSON_GetObjectItem(root, "Close");
    cJSON* cjsonNewClock = cJSON_GetObjectItem(root, "NewClock");

    if (cjsonPause->type == cJSON_String) {
        ePause.push_back(keyshortCharToEnum(cjsonPause->valuestring));
    } else {
        LOG_ERR("cjsonPause type error");
    }

    if (cjsonPause->type == cJSON_String) {
        eCoutTime.push_back(keyshortCharToEnum(cjsonCoutTime->valuestring));
    } else {
        LOG_ERR("cjsonPause type error");
    }

    if (cjsonClose->type == cJSON_String) {
        eClose.push_back(keyshortCharToEnum(cjsonClose->valuestring));
    } else {
        LOG_ERR("cjsonPause type error");
    }

    if (cjsonNewClock->type == cJSON_String) {
        eNewClock.push_back(keyshortCharToEnum(cjsonNewClock->valuestring));
    } else {
        LOG_ERR("cjsonPause type error");
    }
}

void KeyShot::printConfig() {
    LOG_INF("KeyShot Config");
    LOG_INF("ePause: {}", keyshortEnumToString(ePause.at(0)));
    LOG_INF("eCoutTime: {}", keyshortEnumToString(eCoutTime.at(0)));
    LOG_INF("eClose: {}", keyshortEnumToString(eClose.at(0)));
    LOG_INF("eNewClock: {}", keyshortEnumToString(eNewClock.at(0)));
    LOG_INF("");
}

KeyShot::Key KeyShot::keyshortCharToEnum(char* keyshort) {
    if (strcmp(keyshort, "space")) {
        return Key::space;
    } else if (strcmp(keyshort, "ctrl")) {
        return Key::ctrl;
    } else if (strcmp(keyshort, "alt")) {
        return Key::alt;
    } else if (strcmp(keyshort, "esc")) {
        return Key::esc;
    } else if (strcmp(keyshort, "d")) {
        return Key::d;
    } else if (strcmp(keyshort, "n")) {
        return Key::n;
    } else {
        return Key::none;
    }
    return Key::none;
}

std::string KeyShot::keyshortEnumToString(KeyShot::Key keyshort) {
    switch (keyshort) {
        case Key::space:
            return std::string("space");
            break;
        case Key::ctrl:
            return std::string("ctrl");
            break;
        case Key::alt:
            return std::string("alt");
            break;
        case Key::esc:
            return std::string("esc");
            break;
        case Key::d:
            return std::string("d");
            break;
        case Key::n:
            return std::string("n");
            break;
        case Key::none:
            return std::string("non");
            break;
    }
    return std::string("error");
}

ClockAbout::ClockAbout()
    : appName(std::string("")),
      appVersion(std::string("")) {
}

void ClockAbout::loadConfig(cJSON* root) {
    cJSON* cjsonAppName = cJSON_GetObjectItem(root, "AppName");
    cJSON* cjsonAppVersion = cJSON_GetObjectItem(root, "AppVersion");

    if (cjsonAppName->type == cJSON_String) {
        appName = cjsonAppName->valuestring;
    } else {
        LOG_ERR("cjsonAppName type error");
    }

    if (cjsonAppVersion->type == cJSON_String) {
        appVersion = cjsonAppVersion->valuestring;
    } else {
        LOG_ERR("cjsonAppVersion type error");
    }
}

void ClockAbout::printConfig() {
    LOG_INF("ClockAbout Config");
    LOG_INF("appName: {}", appName);
    LOG_INF("appVersion: {}", appVersion);
    LOG_INF("");
}

bool parseJsonBool(cJSON* root, const char* jsonName) {
    if (root->type == cJSON_False) {
        return false;
    } else if (root->type == cJSON_True) {
        return true;
    } else {
        LOG_ERR("{} type error", jsonName);
    }
    return false;
}
