#ifndef GXCODECLOGGER_H
#define GXCODECLOGGER_H

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"

// 日志类型，暂时无用
enum LogType {
    trace,
    debug,
    info,
    warn,
    critical,
    error
};

// 日志的配置项
struct LogConfig {
    LogConfig() {
        level = spdlog::level::debug;  // 日志等级
        path = "log/GXCodecLog.log";
        size = 5 * 1024 * 1024;
        count = 10;
        bTruncate = true;  // 不追加，每次都用新的
    };
    ~LogConfig() = default;

    spdlog::level::level_enum level;  // 当前日志等级
    std::string path;
    int64_t size;
    int count;
    bool bTruncate;  // 是否追加显示
};

class GXCodecLogger {
public:
    static GXCodecLogger* getInstance() {
        if (instanceLogger == nullptr) {
            instanceLogger = new GXCodecLogger();
        }
        return instanceLogger;
    }

    std::shared_ptr<spdlog::logger> getLogger() {  // FIXME 暂时无用
        return logger;
    }

    static void destroyInstance() {
        if (instanceLogger != nullptr) {
            delete instanceLogger;
            instanceLogger = nullptr;
        }
    }

    void logInitialize() {
        // spdlog::cfg::load_env_levels();
        file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(nowConfig->path,
                                                                        !nowConfig->bTruncate);  // 创建文件日志器
        // file_sink->set_level(nowLevel);
        console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();  // 创建颜色控制台日志器
        // console_sink->set_level(nowLevel);
        logger = std::make_shared<spdlog::logger>("logger", spdlog::sinks_init_list{file_sink, console_sink});
        // logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [thread %t] [%^%l%$] [%s:%# %!] %v");
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        logger->set_level(nowConfig->level);           // 只有最终这个才控制输出等级
    };

    std::shared_ptr<spdlog::logger> logger = nullptr;  // FIXME 这是共有的,应该是错误的
private:
    static GXCodecLogger* instanceLogger;
    GXCodecLogger() {
        nowConfig = new LogConfig();
        // testLogger();
        logInitialize();
    };
    ~GXCodecLogger() = default;
    GXCodecLogger(GXCodecLogger const&) = delete;
    void operator=(GXCodecLogger const&) = delete;

    std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink = nullptr;
    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink = nullptr;

    LogConfig* nowConfig;
};

// #define INITLOG(conf)      Logger::getInstance()->Init(conf)
// #define GETLOGLEVEL()      Logger::getInstance()->GetLogLevel()
// #define SETLOGLEVEL(level) Logger::getInstance()->SetLogLevel(level)
#define BASELOG(logInstance, level, ...) \
    (logInstance)->logger->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, level, __VA_ARGS__)
#define LOG_TRC(...) BASELOG(GXCodecLogger::getInstance(), spdlog::level::trace, __VA_ARGS__)
#define LOG_DBG(...) BASELOG(GXCodecLogger::getInstance(), spdlog::level::debug, __VA_ARGS__)
#define LOG_INF(...) BASELOG(GXCodecLogger::getInstance(), spdlog::level::info, __VA_ARGS__)
#define LOG_WRN(...) BASELOG(GXCodecLogger::getInstance(), spdlog::level::warn, __VA_ARGS__)
#define LOG_ERR(...) BASELOG(GXCodecLogger::getInstance(), spdlog::level::err, __VA_ARGS__)
#define LOG_CRT(...) BASELOG(GXCodecLogger::getInstance(), spdlog::level::critical, __VA_ARGS__)

#endif  // GXCODECLOGGER_H
