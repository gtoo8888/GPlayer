#include "WordSql.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    LOG_INF("Test");
    return 0;
}

bool WordSql::initDB(void) {
    LOG_INF("sqlite3_version: {}\n", sqlite3_libversion());

    int rc = sqlite3_open("word.db", &mSqlDB);
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
    bool success = false;
    if (!createWordTable()) {
        return false;
    }
    if (!delWord()) {
        return false;
    }
    if (!updateWord()) {
        return false;
    }
    if (!cleanWordTable()) {
        return false;
    }
    if (!getWordTable()) {
        return false;
    }
    if (mSqlDB != nullptr) {
        closeDB();
    }
    return false;
}

bool WordSql::createWordTable(void) {
    std::string sqlstatement(
        "CREATE TABLE IF NOT EXISTS {:s} ( \
                        id INTEGER PRIMARY KEY AUTOINCREMENT, \
                        word TEXT NOT NULL, \
                        part_of_speech TEXT NOT NULL, \
                        word_translation TEXT NOT NULL, \
                        search_mean TEXT, \
                        meet_time INTEGER, \
                        synonym TEXT, \
                        create_time INTEGER, \
                        modify_time INTEGER, \
                        delete_time INTEGER );");
    char *zErrMsg = nullptr;
    int rc = sqlite3_exec(mSqlDB, sqlstatement.c_str(), callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        LOG_ERR("{} SQL error: {}", sqlstatement, zErrMsg);
        sqlite3_free(zErrMsg);
        return false;
    }
    return true;
}

bool WordSql::addWord(void) {
    return false;
}

bool WordSql::delWord(void) {
    return false;
}

bool WordSql::updateWord(void) {
    return false;
}

bool WordSql::cleanWordTable(void) {
    return false;
}

bool WordSql::getWordTable(void) {
    std::string sqlstatement("select * from test;");
    char *zErrMsg = nullptr;
    int rc = sqlite3_exec(mSqlDB, sqlstatement.c_str(), callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        LOG_ERR("{} SQL error: {}", sqlstatement, zErrMsg);
        sqlite3_free(zErrMsg);
        return false;
    }
    return true;
}
