#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING 1
#include <gtest/gtest.h>
#include <cerrno>
#ifdef _WIN64
#include <io.h>
#else
#include <unistd.h>
#endif
#include "GtooLogger.h"
#include "WordSql.h"
#include "global.h"
#include "sqlite3.h"

// 无效检测方式
void checkCplusplusVersion(void) {
#if __cplusplus == 201703L
    std::cout << "C++17" << std::endl;
#elif __cplusplus == 201402L
    std::cout << "C++14" << std::endl;
#elif __cplusplus == 201103L
    std::cout << "C++11" << std::endl;
#elif __cplusplus == 199711L
    std::cout << "C++98 or C++03" << std::endl;
#else
    std::cout << "pre-standard C++" << std::endl;
#endif
}

class QuickTest : public testing::Test {
protected:
    void SetUp() override {
        start_time_ = time(nullptr);
    }

    void TearDown() override {
        // const time_t end_time = time(nullptr);
        // struct tm *local_start = localtime(&start_time_);
        // struct tm *local_end = localtime(&end_time);

        // char bufStart[80], bufEnd[80];
        // strftime(bufStart, sizeof(bufStart), "%Y-%m-%d %H:%M:%S", local_start);
        // strftime(bufEnd, sizeof(bufEnd), "%Y-%m-%d %H:%M:%S", local_end);
        // printf("start:%s end:%s\n", bufStart, bufEnd);

        // EXPECT_TRUE(end_time - start_time_ <= 5) << "The test took too long.";
    }

    time_t start_time_;
};

class WordSqlUnittest : public QuickTest {
public:
    WordSqlUnittest() {
        mspWordSql = std::make_shared<WordSql>(dbPath, "testWord");
        mspWordSql->initDB();
    };

    ~WordSqlUnittest() {
        mspWordSql->closeDB();
        deleteSQLiteDB(dbPath);
    };

    bool deleteSQLiteDB(std::string path);
    std::shared_ptr<WordSql> mspWordSql;
    std::string dbPath = "testDb.db";
    sqlite3* db;
};

bool WordSqlUnittest::deleteSQLiteDB(std::string path) {
    int32 result;
#ifdef _WIN64
    result = _unlink(path.c_str());
#else
    result = unlink(testInitDbPath.c_str());
#endif

    if (result != 0) {
        LOG_ERR("Unable to delete file: {} err:{}", path, strerror(errno));
        return false;
    } else {
        LOG_INF("Delete file : {} success", path);
        return true;
    }
    return true;
}

TEST_F(WordSqlUnittest, checkCplusplusVersion) {
    checkCplusplusVersion();
}

TEST_F(WordSqlUnittest, initDB_SuccessfulOpen_ReturnsTrue) {
    std::string testInitDbPath = "initDB.db";
    WordSql wordSql(testInitDbPath, "testWord");
    EXPECT_TRUE(wordSql.initDB());
    wordSql.closeDB();
    deleteSQLiteDB(testInitDbPath);
}

TEST_F(WordSqlUnittest, initDB_FailedOpen_ReturnsFalse) {
    WordSql wordSql("./non/existent.db", "testWord");
    EXPECT_FALSE(wordSql.initDB());
}

TEST_F(WordSqlUnittest, CreateWordTable_TableDoesNotExist_TableCreated) {
    EXPECT_TRUE(mspWordSql->createWordTable());
}

TEST_F(WordSqlUnittest, CreateWordTable_TableAlreadyExists_NoError) {
    EXPECT_TRUE(mspWordSql->createWordTable());
    // TODO 是否要验证表存在就不创建
    EXPECT_TRUE(mspWordSql->createWordTable());
}

// TODO模拟 sqlite3_exec 失败
// TEST_F(WordSqlUnittest, CreateWordTable_SqlExecutionFails_ErrorHandled) {
//    //EXPECT_FALSE(mspWordSql->createWordTable());
//}
