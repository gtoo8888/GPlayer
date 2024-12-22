#pragma once

#include "global.h"
#include "sqlite3.h"

class WordSqlInfo {
public:
    WordSqlInfo() = default;
    WordSqlInfo(std::string word,std::string wordTranslation);
    ~WordSqlInfo() = default;
    std::string toStrPrint(void);
    std::string toStrSql(void);

    uint64 id = 0;
    std::string Word;
    std::string WordTranslation;
    std::string PartOfSpeech = std::string("");
    std::string SearchMean = std::string("");
    std::string Synonym = std::string("");
    uint8 MeetTime = 0;
    uint64 CreateTime = 0;
    uint64 ModifyTime = 0;
    uint64 DeleteTime = 0;
};

class WordSql {
public:
    WordSql() = default;
    WordSql(std::string dbPath, std::string tableName);
    ~WordSql() = default;

    bool initDB(void);
    bool closeDB(void);
    bool testDB(void);
    bool createWordTable(void);
    bool insertWord(WordSqlInfo wordSqlInfo);
    WordSqlInfo selectWord(std::string word);
    std::vector<WordSqlInfo> getWordTable(void);
    bool deleteWord(void);
    bool updateWord(void);
    bool cleanWordTable(void);
    

    bool praseError(int32 ret, int8 *stmt, int8 *errStr);
    int64 getNowTime(void);
    WordSqlInfo WordSql::createWordSqlInfoFromStmt(sqlite3_stmt* stmt);


private:
    std::string mDBPath;
    std::string mTableName;
    sqlite3* mSqlDB = nullptr;
};
