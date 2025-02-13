#ifndef GLOBAL_H
#define GLOBAL_H

// 需要包含在最开始处，保证所有文件都看到
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

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>  //define O_WRONLY and O_RDONLY
#include <getopt.h>
#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fstream>

#include <algorithm>
#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include "MediaProfiler.h"
#include "FFmpegDecoder.h"
#include "TranscodingManager.h"
#include "TranscodingTask.h"

#endif  // GLOBAL_H