#pragma once

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef char int8;
typedef short int int16;
typedef int int32;
typedef long long int64;

typedef float float32;
typedef double float64;

//// 无符号整数类型定义
// typedef uint8_t uint8;
// typedef uint16_t uint16;
// typedef uint32_t uint32;
// typedef uint64_t uint64;
//
//// 有符号整数类型定义
// typedef int8_t int8;
// typedef int16_t int16;
// typedef int32_t int32;
// typedef int64_t int64;

#include <stdint.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <QAudioBuffer>
#include <QAudioFormat>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QImage>
#include <QListWidgetItem>
#include <QMediaPlayer>
#include <QMimeData>
#include <QMutex>
#include <QPainter>
#include <QSize>
#include <QString>
#include <QTextCodec>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QWidget>
#include <QtWidgets/QMainWindow>

#include "SDL.h"
// #include "SDL_main.h"

/// 由于是C++的工程,编译的时候使用的C++的编译器编译
/// 而FFMPEG是C的库,需要加上extern "C"
/// 否则会提示各种未定义
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
}

#include "About.h"
#include "GUtils.h"
#include "GtooLogger.h"
#include "PlayList.h"
#include "ReadThread.h"
#include "VideoCtrl.h"
#include "VideoDecode.h"
#include "VoiceDecode.h"
