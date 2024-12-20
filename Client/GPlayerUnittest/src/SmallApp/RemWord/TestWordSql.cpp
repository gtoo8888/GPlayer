#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING 1
#include <gtest/gtest.h>
#include "WordSql.h"

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
     WordSqlUnittest()
         : wordSql(new WordSql) {

           };

     WordSql *wordSql;
 };

TEST_F(WordSqlUnittest, initDB) {
    printf("sqlite3_version: %s\n", sqlite3_libversion());
     EXPECT_TRUE(wordSql->initDB());
}

TEST_F(WordSqlUnittest, Basics2) {
    EXPECT_TRUE(1, 1) << "this is a test";
    EXPECT_TRUE(2, 2) << "this is a test";
}
