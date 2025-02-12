# 如果需要跑起来还需要的东西
1. 3rdparty
   1. ffmpeg
   2. gtest
2. lib的内容


SDL2.lib
SDL2main.lib
SDL2test.lib

# Directory Structure
│  
├─ffmpeg
│      avcodec.lib
│      avdevice.lib
│      avfilter.lib
│      avformat.lib
│      avutil.lib
│      postproc.lib
│      swresample.lib
│      swscale.lib
│      
├─gtest
│      gmock.lib
│      gmock_main.lib
│      gtest.lib
│      gtest_main.lib
│      
├─SDL2
│      SDL2.lib
│      SDL2main.lib
│      SDL2test.lib
│      
└─spdlog
        spdlogd.lib

## Visual Studio 2017 使用SLite3
### 1. 需要的文件
1. [sqlite-amalgamation-3470200.zip][C source code as an amalgamation, version 3.47.2.]
2. [sqlite-dll-win-x64-3470200.zip][64-bit DLL (x64) for SQLite version 3.47.2.]

├─sqlite-amalgamation-3470200
│  shell.c
│  sqlite3.c
│  sqlite3.h
│  sqlite3ext.h
└─sqlite-dll-win-x64-3470200 
   sqlite3.def
   sqlite3.dll


### 2. 将头文件移到指定位置
1. C++->常规->附加包含目录
2. 添加头文件路径

错误操作：
没有添加头文件
报错信息：
错误	C3861	“sqlite3_libversion”: 找不到标识符	

### 3. 生成.lib并移动到指定为止
1. 打开VS提供的终端Visual Studio 2017 Developer Command Prompt v15.9.51  
2. 使用VS提供的链接器，.def->.lib
3. lib /DEF:sqlite3.def /MACHINE:x64
4. 得到sqlite3.lib，sqlite3.exp

### 4. 链接器->常规->附加库目录
1. 添加lib文件路径

错误操作：
没有添加lib文件路径，或者路径错误
报错信息：
错误	LNK2019	无法解析的外部符号 sqlite3_libversion，该符号在函数 "public: __cdecl TestClass::TestClass(class QWidget *)" (??0TestClass@@QEAA@PEAVQWidget@@@Z) 中被引用	
错误	LNK1120	1 个无法解析的外部命令	
错误解释：
1. 会有两个报错
2. 会有很长串的报错
3. 关键词：无法解析的外部符号

错误	LNK1181	无法打开输入文件“D:\Qt\Qt5.9.9\5.9.9\msvc2017_64\lib\Qt5Core.libkernel32.lib”
链接器->输入->附加依赖项，其中的%(AdditionalDependencies)后面没有分号
%(AdditionalDependencies)kernel32.lib

kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib


### 5. 链接器->输入->附加依赖项
1. 添加sqlite3.lib 

错误操作：
添加lib文件路径，没有添加具体lib文件名
报错信息：
错误	LNK1104	无法打开文件“sqlite3.lib”	
错误解释：


### 6. 添加测试内容
1. #include "sqlite3.h"
2. printf("sqlite3_version: %s\n", sqlite3_libversion());


# git submodule更新方法
1. 假设submodule仓库已经有了一个新的commit
2. 进入主仓库
   1. git status
3. 进入子仓库目录
   1. cd 3rdparty
4. git pull不行，会有警告，需要具体的
   1. git pull origin master
5. cd ..
6. git status
   1. 这时候发现 modified:   3rdparty (new commits)
7. git diff
   1. 提交发生了更改
8. git add/commit/pull 将主仓库提交
diff --git a/3rdparty b/3rdparty
index 63646e6..73a2fa5 160000
--- a/3rdparty
+++ b/3rdparty
@@ -1 +1 @@
-Subproject commit 63646e66f045e8fd10d1c64e5c79810be9691082
+Subproject commit 73a2fa5034210636939f49dd9d75414b48858b69



# 参考资料
[SQLite Download Page](https://www.sqlite.org/download.html)
[如何在Windows 10上构建SQLite3.lib文件](https://blog.csdn.net/m0_37909265/article/details/105102982)
[6.DLL中.def文件的使用](https://blog.csdn.net/qq_33757398/article/details/81570004)
[cannot open file 'SOIL.libkernel32.lib'](https://stackoverflow.com/questions/20018396/cannot-open-file-soil-libkernel32-lib)
