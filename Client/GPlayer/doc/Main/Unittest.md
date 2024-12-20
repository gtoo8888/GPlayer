# GPlayerUnittest 单元测试

1. VS2017 单独项目
2. 不依赖 VS2017自带的 Google Test模版
3. 自己创建命令行程序，添加对gtest静态库文件


# 现在问题

1. VS2017中GPlayerUnittest依赖了GPlayer
2. 但是如果要调用GPlayer中函数测试，需要添加源文件到GPlayerUnittest项目中