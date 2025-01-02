# 背单词


# 需求
## TODO TOMORROW
1. 工具
   1. 每一个按钮都需要重新设计进度条
   2. 转化
      1. 处理空行
      2. 处理每一行前后空格去掉
   3. 导入
      1. textbrowse中添加颜色显示
      2. Import按下后，新建文件夹
      3. 有内存没释放
      4. 输出文件应该是UTF-8，输出GB23
2. 单词表
   1. 根据日期选择单词范围
3. 其他
   1. 处理终端输出utf-8异常

## 高优先级

1. 设计功能
   1. 没有背出的单词怎么办
   2. 遗忘曲线怎么添加


## 中优先级
1. 工具
   1. 转化
      1. 对进度条更精细的控制?是否有必要？会影响函数可测试性
      2. 类初始化时，初始化更多文件名
      3. mergeChineseAndEnglishWord中字符串拼接是否可以优化？
      4. 增加运行时间测试
2. 头文件统一管理

## 低优先级


## 高级功能
1. 可以识别字幕中的词
2. 语音识别视频内容，翻译


# 测试
1. 对输入的文件大数量进行测试,6000条左右，瞬间完成

# 记录
1. 保存的单词需要记事本另存为ANSI编码
2. 在会大量重复的for循环内使用Debug，在外部使用info
3. 开启if语句中文会乱码
4. ofstream会中文乱码

参考方式
1. 保存文件为utf-8
2. 终端输出的是乱码就是正确的

# 可读SQL

```sql
CREATE TABLE IF NOT EXISTS WordList (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   word TEXT NOT NULL UNIQUE,
   word_translation TEXT NOT NULL,
   part_of_speech TEXT,
   search_mean TEXT,
   synonym TEXT,
   meet_time INTEGER,
   create_time INTEGER,
   modify_time INTEGER,
   delete_time INTEGER
);
```

# 实际SQL
```sql
CREATE TABLE IF NOT EXISTS WordList (id INTEGER PRIMARY KEY AUTOINCREMENT,word TEXT NOT NULL,word_translation TEXT NOT NULL,part_of_speech TEXT,search_mean TEXT,synonym TEXT,meet_time INTEGER,create_time INTEGER,modify_time INTEGER,delete_time INTEGER);

INSERT INTO WordList (word, word_translation, part_of_speech, search_mean, synonym, meet_time, create_time, modify_time, delete_time) 
values ('test7','中文7','','','',0,1734854494,0,0);

SELECT * FROM WordList;

SELECT * FROM WordList WHERE create_time >= 1735823399 AND create_time <= 1735833599;


DELETE FROM WordList WHERE word = "test7";

UPDATE WordList SET word_translation = '测试2' WHERE word = "test7";
UPDATE WordList SET modify_time = 2 WHERE word = "test7";

DELETE FROM WordList; -- 删除表TxtData数据
UPDATE sqlite_sequence SET id=0 WHERE name='%s'； --更新主键从1开始

```

