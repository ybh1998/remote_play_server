#ifndef DEFINES
#define DEFINES
extern "C"{
#ifdef __cplusplus
 #define __STDC_CONSTANT_MACROS
 #ifdef _STDINT_H
  #undef _STDINT_H
 #endif
 #include <stdint.h>
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#ifndef ANDROID
#include "libavdevice/avdevice.h"
#endif
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}
#include <QString>
const int timeout = 60;
const int backcount = 2;
const int bitrate = 1500000;
const int port=2501;
const int width=800,height=480;
const int buffer=1000000;
const int KEEPALIVE = 0,
    UPDATE = 1,
    LOGIN = 2,
    START = 3,
    STOP = 4,
    MOUSE_MOVE = 5,
    MOUSE_DOWN = 6,
    MOUSE_UP = 7,
    KEY_DOWN = 8,
    KEY_UP = 9,
    VIDEO = 10,
    TYPE = 11;
const int LOGIN_SUCCESS = 0,
    LOGIN_NOUSER = 1,
    LOGIN_PWDERROR = 2,
    LOGIN_TIMEOUT = 3;
const AVCodecID CODEC=AV_CODEC_ID_H264;
const char dns[]="ybh1998.gicp.net";
const int server_num = 1;
const char ver[]="v0.01_r1 http://ybh1998.gicp.net/remote_play_v0.01_r1.apk";
const QString shdir = "/opt/shs/";
#endif // DEFINES

