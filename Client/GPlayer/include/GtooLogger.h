#ifndef GTOOLOGGER_H
#define GTOOLOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"


enum LoggerType      // 日志类型，暂时无用
{
    trace,
    debug,
    info,
    warn,
    critical,
    error
};


struct LogConfig { // 日志的配置项
    LogConfig();
    ~LogConfig();

    spdlog::level::level_enum level;// 当前日志等级
    std::string path;
    int64_t size;
    int count;
    bool bTruncate;// 是否追加显示
};


class GtooLogger
{
public:
    static GtooLogger* getInstance()
    {
        if (instanceLogger == nullptr) {
            instanceLogger = new GtooLogger();
        }
        return instanceLogger;
    }

    std::shared_ptr<spdlog::logger> getLogger() { // FIXME 暂时无用
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

    void logMessage(const LoggerType type,const std::string& message);

    std::shared_ptr<spdlog::logger> logger = nullptr; // FIXME 这是共有的,应该是错误的
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

//#define INITLOG(conf)      Logger::getInstance()->Init(conf)
//#define GETLOGLEVEL()      Logger::getInstance()->GetLogLevel()
//#define SETLOGLEVEL(level) Logger::getInstance()->SetLogLevel(level)
#define BASELOG(logInstance, level, ...) (logInstance)->logger->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, level, __VA_ARGS__)
#define LOG_TRACE(...)     BASELOG(GtooLogger::getInstance(), spdlog::level::trace, __VA_ARGS__)
#define LOG_DEBUG(...)     BASELOG(GtooLogger::getInstance(), spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(...)      BASELOG(GtooLogger::getInstance(), spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(...)      BASELOG(GtooLogger::getInstance(), spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(...)     BASELOG(GtooLogger::getInstance(), spdlog::level::err, __VA_ARGS__)
#define LOG_CRITICAL(...)  BASELOG(GtooLogger::getInstance(), spdlog::level::critical, __VA_ARGS__)


#endif
