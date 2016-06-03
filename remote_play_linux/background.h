#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QObject>
#include <QThread>
#include <QProcess>
#include <QUdpSocket>
#include "defines.h"
extern "C"{
#include <xdo.h>
}

class Background : public QThread
{
public:
    Background(QUdpSocket *socket,int dn);
    ~Background();
    QUdpSocket *socket;
    void mouse_move(int x,int y){
        xdo_move_mouse_relative(xdo,x,y);
    }
    void key_down(char *key){
        //qDebug() << QString("key down %1").arg(key);
        xdo_send_keysequence_window_down(xdo,CURRENTWINDOW,key,0);
    }
    void key_up(char *key){
        //qDebug() << QString("key up %1").arg(key);
        xdo_send_keysequence_window_up(xdo,CURRENTWINDOW,key,0);
    }
    void type(char *key){
        xdo_enter_text_window(xdo,CURRENTWINDOW,key,500);
    }
    void mouse_down(int key){
        xdo_mouse_down(xdo,CURRENTWINDOW,key);
    }
    void mouse_up(int key){
        xdo_mouse_up(xdo,CURRENTWINDOW,key);
    }
    void run();
    QString addr="";
    int port=0;
    int dn;
    void startgame(int index,QString naddr,int nport,QString username);
    void stopgame();
    void send(int id, uint8_t *data, int size);
private:
    char dnc[3]={':',dn+'0',0};
    xdo_t *xdo;
    QProcess xserver,game,kwin;
    AVInputFormat *inputformat;
    AVFormatContext *formatctx = NULL;
    AVDictionary *options = NULL;
    AVCodecContext *codecctx;
    AVCodec *codec;
    AVFrame *frame=avcodec_alloc_frame(),*frameyuv=avcodec_alloc_frame();
    AVCodec *encodec=avcodec_find_encoder(CODEC);
    AVCodecContext *encodecctx=avcodec_alloc_context3(encodec);
    SwsContext *swsctx=sws_getContext(width,height,PIX_FMT_RGB32,
                                          width,height,PIX_FMT_YUV420P,
                                          SWS_BICUBIC,NULL,NULL,NULL);
    AVPacket in_packet,out_packet;
};

#endif // BACKGROUND_H
