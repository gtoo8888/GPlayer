#pragma once

#include "GtooLogger.h"
#include "sqlite3.h"

class WordSql {
public:
    WordSql() = default;
    ~WordSql() = default;

    bool initDB(void);
    bool closeDB(void);
    bool testDB(void);
    bool createWordTable(void);
    bool addWord(void);
    bool delWord(void);
    bool updateWord(void);
    bool cleanWordTable(void);
    bool getWordTable(void);

private:
    sqlite3* mSqlDB = nullptr;
};
