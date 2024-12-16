# 写作日志

## 注意事项
1. 使用vscode远程登录调试的时候，需要用root身份登录，不然无法连接数据库
2. 查找相关的帮助 ```man pthread_cond_broadcast```
3. epoll_event这个结构体返回的类型，在```man epoll_ctl中查看```

# 项目实现顺序
1. 现有一个主函数
main.cpp
2. 主函数写好以后得现有基本的配置
PIGG_config.cpp/.h
3. 各个阶段的初始化都写在这里，虽然现在还没有初始化函数
PIGG_webserver.cpp/.h
4. 先得有日志系统，日志的初始化，日志的输出，先写出来，现在只要能打印就行了（阻塞队列+线程锁后面加）
PIGG_log.cpp/.h
5. *[支线]*零基础做的项目，对有些函数的实现不清楚，所以加了一个PIGG_test.cpp来测试一些小模块的使用
PIGG_test.cpp/.h
6. 补充基础模块，日志的阻塞队列+线程锁
BlockQueue.h+Lock.h
7. http协议的处理
PIGG_http.cpp/.h
8. 补充了PIGG_webserver.cpp中epoll连接的部分
9. 线程池
ThreadPool.h
10. mysql连接池
sql_connection_pool.cpp/.h
11. 使用json作为配置文件
PIGG_webserver_config.json



1.数据库可以最后再写上去，因为可以先不存到数据库
2.写.cpp文件，具体函数的时候，可以先把函数名字写出来，先搭出框架
3.定时器还没写


# 不会的
1.reactor，proactor
2.信号量
3.PIGG_http_conn::process()中的modfd
开个标志位，开始通讯，还是不会搞
4.编译第三方库进cmake到底怎么做，/user/local/目录下的不会用
5.apt安装的文件在哪里
6.cmake到底在哪里找.cmake


# 优化
1. 存放资源文件的目录需要检查是否存在，最差情况也需要报提示
2. 每次新开服务器的时候，应该在日志中给一个明显的提示
3. 可以解析postman的格式


# 扩展

1. 动态资源
2. 如何扩展成HTTPS
3. 小顶堆实现定时器系统
4. 文件上传和下载的功能吗 类似于FTP那种的
5. json解析器


# 参考文献
[日志系统]https://mp.weixin.qq.com/s?__biz=MzI3NzE0NjcwMg==&mid=2650122657&idx=1&sn=c5ce1d8059c40e4cd6deb42a34f8fe49&chksm=f36bb480c41c3d96f69a9fbbc8e7e1515b8bbec87742f76fa3dfda0019a7b58aa282c3ef9bde&scene=21#wechat_redirect</br>
[Reactor模型和Proactor模型]https://cloud.tencent.com/developer/article/1488120</br>
[Reactor模型]https://www.cnblogs.com/CodeBear/p/12567022.html</br>
[C++检查文件是否存在]https://blog.csdn.net/guotianqing/article/details/100766120</br>
[多线程编程之条件变量]https://blog.csdn.net/lovecodeless/article/details/24929273</br>
[Cmakelists.txt添加jsoncpp库]https://blog.csdn.net/dtjun_sjtu/article/details/122683547</br>
[vscode无法识别头文件“cannot open source file “gnu/stubs-64.h“”]https://blog.csdn.net/miraclequan/article/details/122822287
[Linux下编译安装JsonCpp(1.9.4)最新版本]https://blog.csdn.net/weixin_44290011/article/details/109120478
https://subingwen.cn/cpp/jsoncpp/#2-jsoncpp-%E7%9A%84%E4%BD%BF%E7%94%A8




##### 参考的github仓库
[TinyWebServer]https://github.com/qinguoyi/TinyWebServer</br>
[HAHA-WebServer里面添加了json配置]https://github.com/tomatowithpotato/HAHA-WebServer</br>
[markparticle/WebServer]https://github.com/markparticle/WebServer</br>
[本项目是一个基于Linux 的轻量级Web服务器，能够对浏览器的连接请求进行解析处理，处 理完之后给浏览器客户端返回一个响应，如文字、图片、视频等;
小顶堆实现定时器系统]https://github.com/huboy-zhao/MyWebserver</br>
[使用C++11实现数据库连接池连接数据库]https://github.com/xbQAQ/httpServer</br>