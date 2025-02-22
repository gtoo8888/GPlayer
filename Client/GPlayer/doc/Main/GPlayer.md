# 需求
集成萤石api


# 未来需要做的
1. 完善视频信息的结构体，参考potplay播放过程，查看视频播放过程还有什么信息
    AVFormatContext， bit_rate 比特率
2. 可以拖动进度条
3. 可以播放声音
4. 播放过程打印信息，显示日志
5. 播放过程显示统计信息
6. 结束时候有异常，有资源没有释放？


# 各个区域的功能
1. 播放列表
   1. 鼠标拖动要播放的文件，调整顺序
   2. 播放下一个
   3. 播放上一个
   4. 拖放处理？？什么东西？
   5. 下面的按钮无法兼容上面的播放
   6. 视频列表切换的时候有卡顿

# 需要完善的功能
1. 暂停
2. 视频设置
3. 关闭
4. 退出
5. 全屏


# 优先级较低的
1. 统计代码行数工具-大致完成，只能统计两级目录
2. 跨平台
   1. 使用CMake在ubuntu下编译
   2. qmake在windows平台生成


# 视频解码流程
打开输入流：使用FFmpeg等库提供的函数，如avformat_open_input，打开视频文件或流，获取解封装上下文。
读取流信息：使用avformat_find_stream_info函数读取媒体文件的数据包，获取视频流的基本信息，如时长、帧率等。
选择视频流：使用av_find_best_stream等函数，确定视频流的索引，以便后续获取详细的视频流信息。
获取视频流详细信息：通过解封装上下文得到的视频流索引，获取视频流的详细信息，如分辨率、帧率、总帧数、解码器等。
初始化解码器上下文：根据视频流的解码器ID，使用avcodec_alloc_context3分配解码器上下文，并进行必要的初始化。
设置解码器参数：使用avcodec_parameters_to_context将视频流的参数复制到解码器上下文中，为解码做好准备。
打开解码器：使用avcodec_open2打开视频解码器，使其处于可用状态。
分配缓冲区：根据视频流的属性，使用av_image_get_buffer_size等函数计算缓冲区的大小，并分配足够的内存用于存储解码后的图像数据。
解码：进入解码循环，使用av_read_frame读取视频帧的压缩数据，然后使用avcodec_receive_frame进行解码，得到解码后的图像数据。
后续处理：可以对解码后的图像进行进一步处理，如渲染、显示或保存等。
释放资源：在视频解码结束后，需要释放申请的资源，包括解码器上下文、缓冲区等。


# 参考资料
[Qt音视频开发22-音频播放QAudioOutput](https://zhuanlan.zhihu.com/p/612782041)
[【FFmpeg视频播放器开发】解封装解码流程、常用API和结构体简介（一） ](https://www.cnblogs.com/linuxAndMcu/p/14603442.html)
https://david1840.github.io/2019/04/19/SDL2%E9%9F%B3%E9%A2%91%E6%92%AD%E6%94%BE/
[M了个J【秒懂音视频开发】](https://www.cnblogs.com/mjios/p/?page=2)
[ffmpeg 32位](https://github.com/sudo-nautilus/FFmpeg-Builds-Win32)
[关于Visual Studio无法高亮显示问题（非设置问题）](https://blog.csdn.net/u010635451/article/details/79616309)
