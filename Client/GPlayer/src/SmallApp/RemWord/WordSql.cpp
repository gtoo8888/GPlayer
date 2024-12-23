#include "WordSql.h"

WordSql::WordSql(std::string dbPath, std::string tableName)
    : mDBPath(dbPath),
      mTableName(tableName) {
}

bool WordSql::initDB(void) {
    LOG_INF("sqlite3_version: {}", sqlite3_libversion());

    int rc = sqlite3_open(mDBPath.c_str(), &mSqlDB);
    if (rc != 0) {
        LOG_ERR("Can't open database: %s\n", sqlite3_errmsg(mSqlDB));
        sqlite3_close(mSqlDB);
        return false;
    }
    return true;
}

bool WordSql::closeDB(void) {
    sqlite3_close(mSqlDB);
    return true;
}

bool WordSql::testDB(void) {
    if (!createWordTable()) {
        return false;
    }

    // VS2017特殊处理，需要代码页改为GB2312,SQLite3会得到BLOB，二进制大文件
    // 需要使用utf-8，虽然调试乱码，但是写入正确
    // WordSqlInfo wordSqlInfo("test", "中文");
    // if (!insertWord(wordSqlInfo)) {
    //    return false;
    //}
    // WordSqlInfo wordSqlInfo2("test2", "中文二");
    // insertWord(wordSqlInfo2);
    // WordSqlInfo wordSqlInfo3("test3", "中文三");
    // insertWord(wordSqlInfo3);
    // WordSqlInfo wordSqlInfo4("test4", "中文四");
    // insertWord(wordSqlInfo4);
    // WordSqlInfo wordSqlInfo5("test6", "中文五");
    // insertWord(wordSqlInfo5);

    // std::vector<WordSqlInfo> tvWordList = getWordTable();
    // if (tvWordList.size() == 0) {
    //     return false;
    // }
    // WordSqlInfo tWord = selectWord("test6");
    // WordSqlInfo tWord = selectWord("test5");
    // if (tWord.id == 0) {
    //    return false;
    //}

    if (!deleteWord("test5")) {
        return false;
    }

    updateWordTranslation("test4", "测试123");
    updateModifyTime("test4", 123);
    updateModifyTimeNow("test4");


    if (!cleanWordTable()) {
        return false;
    }
    if (mSqlDB != nullptr) {
        closeDB();
    }
    return false;
}

bool WordSql::praseError(int32 ret, int8 *stmt, int8 *errStr) {
    if (ret != SQLITE_OK) {
        LOG_ERR("{} SQL error: {}", stmt, errStr);
        sqlite3_free(errStr);
        return false;
    }
    LOG_DBG("Success: {}", stmt);
    return true;
}

int64 WordSql::getNowTime(void) {
    // 需要用system_clock获取UNXI时间,high_resolution_clock不可以
    std::chrono::system_clock::time_point nowTime = std::chrono::system_clock::now();
    std::chrono::nanoseconds duratimSinceEpoch = nowTime.time_since_epoch();
    // 转化为秒
    std::chrono::seconds secSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(duratimSinceEpoch);
    // 转化为更高精度时间
    std::chrono::duration<double> fractionalSec =
        std::chrono::duration_cast<std::chrono::duration<double>>(duratimSinceEpoch);

    int64 secSinceEpoch64 = static_cast<int64>(secSinceEpoch.count());
    std::string secSinceEpochStr = std::to_string(secSinceEpoch64);

    LOG_DBG("Current Unix timestamp in seconds: {}", secSinceEpoch64);
    LOG_DBG("Current Unix timestamp with fractions of a second: {}", fractionalSec.count());

    return secSinceEpoch64;
}

// sqlStmt - 简单且直观，保留了"statement"的含义。
// stmt - 如果上下文已经很清楚这是SQL语句的话，可以进一步缩短。
// sqlStr - 强调这是一个SQL字符串。
// query 或 sqlQuery - 如果你习惯使用"query"来指代SQL语句。
bool WordSql::createWordTable(void) {
    int8 sqlStmt[512];
    // TODO 验证UNIQUE
    snprintf(sqlStmt, sizeof(sqlStmt),
             "CREATE TABLE IF NOT EXISTS %s ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT,"
             "word TEXT NOT NULL UNIQUE,"
             "word_translation TEXT NOT NULL,"
             "part_of_speech TEXT,"
             "search_mean TEXT,"
             "synonym TEXT,"
             "meet_time INTEGER,"
             "create_time INTEGER,"
             "modify_time INTEGER,"
             "delete_time INTEGER);",
             mTableName.c_str());

    char *zErrMsg = nullptr;
    int32 ret = sqlite3_exec(mSqlDB, sqlStmt, nullptr, 0, &zErrMsg);
    return praseError(ret, sqlStmt, zErrMsg);
}

bool WordSql::insertWord(WordSqlInfo wordSqlInfo) {
    wordSqlInfo.CreateTime = getNowTime();
    LOG_DBG("{}", wordSqlInfo.toStrPrint());

    int8 sqlStmt[512];
    snprintf(sqlStmt, sizeof(sqlStmt),
             "INSERT INTO %s (word, word_translation, part_of_speech, search_mean, synonym, meet_time, create_time, "
             "modify_time, delete_time) values (%s);",
             mTableName.c_str(), wordSqlInfo.toStrSql().c_str());

    char *zErrMsg = nullptr;
    int ret = sqlite3_exec(mSqlDB, sqlStmt, nullptr, 0, &zErrMsg);
    return praseError(ret, sqlStmt, zErrMsg);
}

std::vector<WordSqlInfo> WordSql::getWordTable(void) {
    int8 sqlStmt[512];
    snprintf(sqlStmt, sizeof(sqlStmt), "SELECT * FROM %s;", mTableName.c_str());

    sqlite3_stmt *stmt;
    std::vector<WordSqlInfo> vSelectWordSqlInfo;
    if (sqlite3_prepare_v2(mSqlDB, sqlStmt, -1, &stmt, nullptr) != SQLITE_OK) {
        LOG_ERR("Failed to prepare statement: {}", sqlite3_errmsg(mSqlDB));
        return vSelectWordSqlInfo;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        WordSqlInfo wordSqlInfo = createWordSqlInfoFromStmt(stmt);
        LOG_INF("{}", wordSqlInfo.toStrPrint());
        vSelectWordSqlInfo.push_back(wordSqlInfo);
    }

    sqlite3_finalize(stmt);
    return vSelectWordSqlInfo;
}

WordSqlInfo WordSql::createWordSqlInfoFromStmt(sqlite3_stmt *stmt) {
    int columnCount = sqlite3_column_count(stmt);
    WordSqlInfo wordSqlInfo;

    for (int i = 0; i < columnCount; ++i) {
        const unsigned char *value = sqlite3_column_text(stmt, i);
        std::string valStr(reinterpret_cast<const char *>(value));
        // LOG_DBG("i:{} value:{}", i, valStr);
        // clang-format off
        switch (i) {
            case 0: wordSqlInfo.id = std::stoull(valStr); break;
            case 1: wordSqlInfo.Word = valStr; break;
            case 2: wordSqlInfo.WordTranslation = valStr; break;
            case 3: wordSqlInfo.PartOfSpeech = valStr; break;
            case 4: wordSqlInfo.SearchMean = valStr; break;
            case 5: wordSqlInfo.Synonym = valStr; break;
            case 6: wordSqlInfo.MeetTime = static_cast<uint8>(std::stoi(valStr)); break;
            case 7: wordSqlInfo.CreateTime = std::stoull(valStr); break;
            case 8: wordSqlInfo.ModifyTime = std::stoull(valStr); break;
            case 9: wordSqlInfo.DeleteTime = std::stoull(valStr); break;
            default:
                LOG_WRN("Invalid item");
        }
        // clang-format on
    }
    return wordSqlInfo;
}

WordSqlInfo WordSql::selectWord(std::string word) {
    int8 sqlStmt[512];
    snprintf(sqlStmt, sizeof(sqlStmt), "SELECT * FROM %s WHERE %s = '%s';", mTableName.c_str(), "word", word.c_str());

    sqlite3_stmt *stmt;
    WordSqlInfo tmpWordSqlInfo;
    if (sqlite3_prepare_v2(mSqlDB, sqlStmt, -1, &stmt, nullptr) != SQLITE_OK) {
        return tmpWordSqlInfo;
    }

    sqlite3_bind_text(stmt, 1, word.c_str(), -1, SQLITE_STATIC);

    int rowNum = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        rowNum++;
    }
    LOG_INF("rowNum:{}", rowNum);
    // 先把同个单词存多次的剔除掉
    if (rowNum > 1) {
        LOG_WRN("Word:{} RowNum:{} Multiple items are stored for the same word.", word, rowNum);
        return tmpWordSqlInfo;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        tmpWordSqlInfo = createWordSqlInfoFromStmt(stmt);
    }
    LOG_INF("{}", tmpWordSqlInfo.toStrPrint());
    sqlite3_finalize(stmt);
    return tmpWordSqlInfo;
}

// TODO 判断没有删除
bool WordSql::deleteWord(std::string word) {
    int8 sqlStmt[512];
    snprintf(sqlStmt, sizeof(sqlStmt), "DELETE FROM %s WHERE word = '%s'", mTableName.c_str(), word.c_str());

    char *zErrMsg = nullptr;
    int ret = sqlite3_exec(mSqlDB, sqlStmt, nullptr, 0, &zErrMsg);
    return praseError(ret, sqlStmt, zErrMsg);
}

bool WordSql::updateWordTranslation(std::string word, std::string wordTranslation) {
    int8 sqlStmt[512];
    snprintf(sqlStmt, sizeof(sqlStmt), "UPDATE %s SET word_translation = '%s' WHERE word = '%s'", mTableName.c_str(),
             wordTranslation.c_str(), word.c_str());

    char *zErrMsg = nullptr;
    int ret = sqlite3_exec(mSqlDB, sqlStmt, nullptr, 0, &zErrMsg);
    return praseError(ret, sqlStmt, zErrMsg);
}

bool WordSql::updateModifyTime(std::string word, int64 time) {
    int8 sqlStmt[512];
    snprintf(sqlStmt, sizeof(sqlStmt), "UPDATE %s SET modify_time = %lld WHERE word = '%s'", mTableName.c_str(),
        time, word.c_str());

    char *zErrMsg = nullptr;
    int ret = sqlite3_exec(mSqlDB, sqlStmt, nullptr, 0, &zErrMsg);
    return praseError(ret, sqlStmt, zErrMsg);
}

bool WordSql::updateModifyTimeNow(std::string word) {
    int64 nowTime = getNowTime();
    int8 sqlStmt[512];
    snprintf(sqlStmt, sizeof(sqlStmt), "UPDATE %s SET modify_time = %lld WHERE word = '%s'", mTableName.c_str(),
        nowTime, word.c_str());

    char *zErrMsg = nullptr;
    int ret = sqlite3_exec(mSqlDB, sqlStmt, nullptr, 0, &zErrMsg);
    return praseError(ret, sqlStmt, zErrMsg);
}

bool WordSql::cleanWordTable(void) {
    int8 sqlStmt[512];
    snprintf(sqlStmt, sizeof(sqlStmt), "DELETE FROM %s", mTableName.c_str());

    char *zErrMsg = nullptr;
    int ret = sqlite3_exec(mSqlDB, sqlStmt, nullptr, 0, &zErrMsg);
    if (praseError(ret, sqlStmt, zErrMsg) == false) {
        return false;
    }

    memset(sqlStmt, 0x00,sizeof(sqlStmt));
    snprintf(sqlStmt, sizeof(sqlStmt), "UPDATE sqlite_sequence SET id=0 WHERE name='%s'", mTableName.c_str());

    ret = sqlite3_exec(mSqlDB, sqlStmt, nullptr, 0, &zErrMsg);
    if (praseError(ret, sqlStmt, zErrMsg) == false) {
        return false;
    }
}

bool WordSql::deleteWordTable(void)
{
    return false;
}

WordSqlInfo::WordSqlInfo(std::string word, std::string wordTranslation)
    : Word(word),
      WordTranslation(wordTranslation) {
}

std::string WordSqlInfo::toStrPrint(void) {
    char sqlChar[512];
    snprintf(sqlChar, sizeof(sqlChar),
             "id:%d,Word:%s,Translation:%s,PartOfSpeech:%s,SearchMean:%s,Synonym:%s,MeetTime:%d,CreateTime:%lld,"
             "ModifyTime:%lld,DeleteTime:%lld",
             id, Word.c_str(), WordTranslation.c_str(), PartOfSpeech.c_str(), SearchMean.c_str(), Synonym.c_str(),
             MeetTime, CreateTime, ModifyTime, DeleteTime);
    std::string sqlStr(sqlChar);
    return sqlStr;
}

std::string WordSqlInfo::toStrSql(void) {
    char sqlChar[512];
    snprintf(sqlChar, sizeof(sqlChar), "'%s','%s','%s','%s','%s',%d,%lld,%lld,%ld", Word.c_str(),
             WordTranslation.c_str(), PartOfSpeech.c_str(), SearchMean.c_str(), Synonym.c_str(), MeetTime, CreateTime,
             ModifyTime, DeleteTime);
    std::string sqlStr(sqlChar);
    return sqlStr;
}
