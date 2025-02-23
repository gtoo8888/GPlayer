# CHANGELOG

Sun Feb 23 2025 16:18 - v0.0.4
  - refactor: 整理头文件，全都统一到global.h中
  - feat: 音频解码VoiceDecode拆分成多个函数
  - fix: VoiceDecode根据输入确定输出采样率，解决播放声音毛刺问题，但是播放时间长还是有问题

Sat Feb 22 2025 23:54 - v0.0.3
  - feat: 新开线程ReadVideoThread，同时播放视频音频

Wed Feb 13 2025 01:02 - v0.0.2
  - refactor: GPlayer.ui结构，播放列表最开始不显示
  - refactor: 修改变量名btnXX，修改函数名slotXX，修改注释
  - chore: 支持VS2017编译Release版本，去掉了gtest
