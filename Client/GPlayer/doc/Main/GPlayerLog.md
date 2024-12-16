[GPlayerServer][Old PIGG_webserver]
## 2022-7-24
1. task.josn和lanch.json可以跑起来了，可以用vscode远程连接调试
2. 使用cmake进行编译
3. cmake编译中添加了mysql,可以操作数据库了

## 2022-7-25
1. 学习了namespace和share_ptr是怎么使用的

## 2022-7-30
1. 之前发现使用语句，但是日志无法写入
fix:使用阻塞队列插入，但是自己还没有实现阻塞队列的内容，导致一直是空实现，就没法写进去了
2. 遗留的问题：单例模式在vscode中如何调试，因为进去就是一个instant()函数了

## 2022-7-30
1. 实现了阻塞队列BlockQueue.h和线程锁Lock.h，但是都还没有开始使用，现在都是没有线程不安全的

## 2022-7-31
1. 增加了mysql连接池的构造函数SqlConnPool.h

## 2022-9-16
1. 修改CMake文件http和sql_connection加入编译，修改语法问题
2. 增加github中的workflow自动化测试

## 2022-9-18
1. 写具体的http的内容，函数体都写好了，完成了http应答的部分

## 2022-9-19
1. 继续写具体的http的内容，http的读取
2. 增加了HTML页面

## 2022-9-20
1. 增加了timer的内容，项目中自己写的一个定时器
2. 主函数运行中的webserver中增加了定时器处理的功能
3. 梳理主函数运行流程，接下来准备要跑起来了

## 2022-9-22
1. 程序跑起来，进入主循环event_loop()没有问题，但是使用postman发送数据，就导致程序Segmentation fault (core dumped)错误
2. 卡在epoll_wait里面，正确的程序可以一直循环，怀疑是没有开线程池的原因，所以一直在等待
3. PIGG_http_conn::read_once()函数没有编写没法使用LT和ET读取数据
4. 复习了epoll的使用流程，复习epoll相关函数

## 2022-9-23
1. 增加了很多八股文
2. 学习定时器的相关操作，了解时间轮的原理

## 2022-9-27
1. 增加了线程池，增加了连接池

# 2022-10-11
1. 修复fopen无法打开日志的问题，由于中间文件夹不存在
2. 现在连接，日志正常打印，浏览器没有显示，报出PIGG_http_conn::close_conn close 16问题

# 2022-10-20
1. 第一次调试成功，问题出在存在html文件的目录是写死的，抄的老代码用的"/root"
在找不到html页面的时候会返回NO_SOURCE，但是不会有日志和打印输出所以一直找不到问题
2. 现在总是刷新页面会有的时候就无法访问，不知道是什么原因
3. 使用账号登录的时候会有问题

# 2022-10-21
1. 登录的时候数据库出现问题，怀疑是线程池调试有问题，现在看到MYSQL没有创建对象
2. 创建了str和pthread的测试程序

# 2022-10-22
1. 增加了jsoncpp库，用json文件来保存配置文件，增加了json的练习，将json加入cmake编译，将配置程序单独存放起来
2. 再修复了一下html网页的文件找不到的问题，之前strcat抄成了strcpy

# 2022-03-12
1. 增加了C++连接redis的模块，写了一个测试文件
2. 发现了日志模块是无法进行关闭的，只能手动关闭
3. 重新进行了压测，1W并发时没有问题，当最大3W时，有15%的错误，都是测试60s
4. 重新修改了目录结构
5. 修改了html文件，尝试增加一个json转xml的页面

[GPlayer][Old GtooPlayer]
# 2023-12-04
1. 建立仓库

# 2023-12-05
1. 参考别人的代码，主界面可以播放视频
2. 尝试解决utf-8乱码问题，后来发现是保存的时候需要保存为utf-8

# 2023-12-06
1. 增加统计代码行数python工具
2. 使用gtest做测试
3. 尝试在ubuntu中，使用cmake编译，只能实现一个简单的例子，但是可以跑起来
4. 使用gcov,lcov覆盖率测试

# 2023-12-07
1. 重新更改lib的目录
2. 语言规范
3. QLable显示当前播放时间
4. QPregressBar可以显示进度条
5. 增加了播放列表PlayList，点击可以播放视频

# 2023-12-08
1. 增加spdlog作为日志
2. 尝试读取音量，了解ffmpeg解码的各个接口
3. 增加了视频控制窗口，控制饱和度色彩，控制速度
4. 重新规划了布局，主界面可以根据控件大小伸缩了，修改了控件的名称，符合规范

[GPlayer]
# 2024-12-13
1. 迁移代码，来自
   1. 原始的VS2017播放器-GPlayer
   2. Qt-SmallAPP-Clock的秒表功能-GPlayer-SmallApp
   3. PIGG_webserver-GPlayerServer
2. 将代码同步到Github,Gitee,GitLab
3. [GPlayer] 需要依赖第三方库的lib,将3rdparty submodule化到GitLab中
4. [GPlayerServer] 修复运行bug,clang-format格式化

# 2024-12-14
1. [GPlayerServer] 修改CMake构建流程，从src构建转换到在根目录构建
2. [GPlayerServer] 重构文件名、目录结构
3. [GPlayer][RemWord] 开发Tool中，原始文件Transform功能，主要处理ifstream,fgets,QFile读取文件中文乱码问题