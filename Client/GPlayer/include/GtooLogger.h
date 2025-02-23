#ifndef GTOOLOGGER_H
#define GTOOLOGGER_H

#include "global.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"

enum LoggerType  // 日志类型，暂时无用
{
    trace,
    debug,
    info,
    warn,
    critical,
    error
};

struct LogConfig {  // 日志的配置项
    LogConfig();
    ~LogConfig() = default;

    spdlog::level::level_enum level;  // 当前日志等级
    std::string path;
    int64_t size;
    int count;
    bool bTruncate;  // 是否追加显示
};

class GtooLogger {
public:
    static GtooLogger* getInstance() {
        if (instanceLogger == nullptr) {
            instanceLogger = new GtooLogger();
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

    void logInitialize();
    void testLogger();

    void logMessage(const LoggerType type, const std::string& message);

    std::shared_ptr<spdlog::logger> logger = nullptr;  // FIXME 这是共有的,应该是错误的
private:
    static GtooLogger* instanceLogger;
    GtooLogger();
    ~GtooLogger();
    GtooLogger(GtooLogger const&) = delete;
    void operator=(GtooLogger const&) = delete;

    std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink = nullptr;
    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink = nullptr;

    LogConfig* nowConfig;
};

// #define INITLOG(conf)      Logger::getInstance()->Init(conf)
// #define GETLOGLEVEL()      Logger::getInstance()->GetLogLevel()
// #define SETLOGLEVEL(level) Logger::getInstance()->SetLogLevel(level)
#define BASELOG(logInstance, level, ...) \
    (logInstance)->logger->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, level, __VA_ARGS__)
#define LOG_TRC(...) BASELOG(GtooLogger::getInstance(), spdlog::level::trace, __VA_ARGS__)
#define LOG_DBG(...) BASELOG(GtooLogger::getInstance(), spdlog::level::debug, __VA_ARGS__)
#define LOG_INF(...) BASELOG(GtooLogger::getInstance(), spdlog::level::info, __VA_ARGS__)
#define LOG_WRN(...) BASELOG(GtooLogger::getInstance(), spdlog::level::warn, __VA_ARGS__)
#define LOG_ERR(...) BASELOG(GtooLogger::getInstance(), spdlog::level::err, __VA_ARGS__)
#define LOG_CRT(...) BASELOG(GtooLogger::getInstance(), spdlog::level::critical, __VA_ARGS__)

#endif
