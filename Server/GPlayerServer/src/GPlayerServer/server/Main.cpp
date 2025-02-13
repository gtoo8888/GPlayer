#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <memory>  //shared_ptr
#include <sstream>
#include <string>
#include "Config.h"
#include "Lock.h"
#include "Logger.h"
#include "PIGG_test.h"
#include "Webserver.h"

void test_mysql() {
    std::string user = "root";
    std::string passwd = "root";
    std::string databasename = "yourdb";

    MYSQL *conn = NULL;
    conn = mysql_init(conn);

    int port = 3306;
    MYSQL_RES *res;
    MYSQL_ROW row;

    /* 连接数据库 */
    if (!mysql_real_connect(conn, "localhost", user.c_str(), passwd.c_str(), databasename.c_str(), port, NULL, 0)) {
        std::cout << mysql_error(conn);
        return;
    }

    for (int i = 0; i < 100; i++) {
        char name[100] = "56";
        char password[100] = "TTT";
        std::string test;

        // std::ostringstream ostr,ostr2;
        // ostr << i+1;
        // std::string astr1 = ostr.str();
        // ostr2 << i;
        // std::string astr2 = ostr2.str();

        // test = astr1 + "&" +  astr2;

        // char *m_string = &test[0]; //存储请求头数据
        // int j = 0;
        // for ( ;m_string[j] != '&'; ++j)
        //     name[j - 5] = m_string[i];
        // name[j - 5] = '\0';

        // int j = 0;
        // for (; m_string[j] != '\0'; ++j)
        //     password[j] = m_string[j];
        // password[j] = '\0';

        char *sql_insert = (char *)malloc(sizeof(char) * 200);
        strcpy(sql_insert, "INSERT INTO user(username, passwd) VALUES(");
        strcat(sql_insert, "'");
        strcat(sql_insert, name);
        strcat(sql_insert, "', '");
        strcat(sql_insert, password);
        strcat(sql_insert, "')");
        // std::cout << sql_insert << std::endl;
        if (mysql_query(conn, sql_insert)) {
            std::cout << mysql_error(conn);
            return;
        }
    }

    /* 查询语句 */
    if (mysql_query(conn, "select * from user")) {
        std::cout << mysql_error(conn);
        return;
    }

    res = mysql_use_result(conn);

    /* 打印查询结果 */
    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%s %s\n", row[0], row[1]);
    }

    /* 断开连接 */
    mysql_free_result(res);
    mysql_close(conn);
}

void test_all() {
    PIGG_init_test::PIGG_test my_test;  // 专门用来测试的函数

    // my_test.test_callback1();
    // my_test.test_callback3();
    // my_test.test_callback4();
    // my_test.test_callback5();

    // my_test.test_redis_command();
    exit(0);  // 仅仅为了跑测试程序
}

int main(int argc, char *argv[]) {
    Config config;
    GPlayerServer server(config);
    server.event_loop();
    return 0;
}
