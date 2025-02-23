#include "GtooLogger.h"

#include "spdlog/cfg/env.h"   // support for loading levels from the environment variable
#include "spdlog/cfg/env.h"
#include "spdlog/fmt/ostr.h"  // support for user defined types

GtooLogger* GtooLogger::instanceLogger = nullptr;

LogConfig::LogConfig() {
    level = spdlog::level::debug;  // 日志等级
    path = "log/gtoo_log.log";
    size = 5 * 1024 * 1024;
    count = 10;
    bTruncate = true;  // 不追加，每次都用新的
}

GtooLogger::GtooLogger() {
    nowConfig = new LogConfig();

    // testLogger();
    logInitialize();
}

void GtooLogger::logInitialize(void) {
    spdlog::cfg::load_env_levels();
    file_sink =
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(nowConfig->path, !nowConfig->bTruncate);  // 创建文件日志器
    // file_sink->set_level(nowLevel);
    console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();  // 创建颜色控制台日志器
    // console_sink->set_level(nowLevel);
    logger = std::make_shared<spdlog::logger>("logger", spdlog::sinks_init_list{file_sink, console_sink});
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [thread %t] [%^%l%$] [%s:%# %!] %v");
    logger->set_level(nowConfig->level);  // 只有最终这个才控制输出等级
}

void GtooLogger::testLogger(void) {
    spdlog::cfg::load_env_levels();  // ?
    // false表示追加模式，true表示覆盖模式
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt", true);  // 创建文件日志器
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();  // 创建颜色控制台日志器
    // auto console_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();// 创建控制台日志器
    console_sink->set_level(spdlog::level::info);
    auto logger = std::make_shared<spdlog::logger>("logger", spdlog::sinks_init_list{file_sink, console_sink});
    // %z表示时区
    // %l表示长日志级别，%L表示短日志级别
    // %v表示具体输出内容
    // %t线程号，%P进程号
    // [%s] 文件
    // [%#] 行号
    // [%!] 函数
    // logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e %z] [thread %t] [process %P] [%^%l%$] %v");// 显示信息最全的
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [thread %t] [%^%L%$] [%s:%# %!] v");

    logger->set_pattern("%+");  // 不知道什么作用

    logger->info("Hello World!\n");
    logger->info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
    logger->debug("This message should not be displayed!");
    logger->info("This an info message with custom format");
    logger->warn("Easy padding in numbers like {:08d}", 12);
    logger->critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    logger->info("Support for floats {:03.2f}", 1.23456);
    logger->info("Positional args are {1} {0}..", "too", "supported");
    logger->info("{:>8} aligned, {:<8} aligned", "right", "left");

    logger->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, spdlog::level::info, "test");

    // 运行时候日志等级调整
    logger->set_level(spdlog::level::info);   // Set global log level to info
    logger->debug("This message should not be displayed!");
    logger->set_level(spdlog::level::trace);  // Set specific logger's log level
    logger->debug("This message should be displayed..");
}

void testLog(void) {
    LOG_DBG("中文");
    LOG_TRC("trace");
    LOG_DBG("debug");
    LOG_DBG("info");
    LOG_WRN("warn");
    LOG_ERR("error");
    LOG_CRT("critical");

    LOG_DBG("PID:[{:08d}] {:08d} ", 123, 123);
    LOG_DBG("str:{}", "test");
    QString qstr("qstr");
    LOG_DBG("qstr:{}", qstr.toStdString());  // 不能直接输出QString
}
