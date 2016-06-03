#include "background.h"
#include "../include/defines.h"
#include <cstdio>

Background::Background(QUdpSocket *socket, int dn)
    :socket(socket),dn(dn)
{
    xserver.start(QString(shdir+"xserver.sh %1 %2 %3").arg(dn).arg(width).arg(height));
    QThread::sleep(1);
    kwin.start(QString(shdir+"kwin.sh %1").arg(dn));
    start();
}
Background::~Background(){
    xserver.kill();
    kwin.kill();
    game.kill();
    quit();
    xdo_free(xdo);
    av_free(swsctx);
    av_free(encodec);
    av_free(encodecctx);
    av_free(frame);
    av_free(frameyuv);
    av_free(codec);
    av_free(codecctx);
    av_free(formatctx);
    av_free(options);
    av_free(inputformat);
    av_free_packet(&in_packet);
    av_free_packet(&out_packet);
}
void Background::send(int id, uint8_t *data, int size){
    uint8_t *tmpdata = (uint8_t *)malloc(size+4);
    *((int *)tmpdata)=id;
    memcpy((void *)(tmpdata+4),(void *)data,size);
    socket->writeDatagram((char *)tmpdata,size+4,QHostAddress(addr),port);
}
void Background::run(){
    QThread::sleep(3);
    xdo=xdo_new(dnc);
    inputformat = av_find_input_format("x11grab");
    int got;
    char sizeopt[20];
    sprintf(sizeopt,"%dx%d",width,height);
    av_dict_set(&options,"video_size",sizeopt,0);
    av_dict_set(&options,"framerate","8",0);
    got=avformat_open_input(&formatctx,dnc,inputformat,&options);
    avformat_find_stream_info(formatctx,&options);
    codecctx=formatctx->streams[0]->codec;
    codec=avcodec_find_decoder(codecctx->codec_id);
    avcodec_open2(codecctx,codec,NULL);
    //av_opt_set(encodecctx->priv_data, "preset", "slow", 0);
    encodecctx->time_base = (AVRational){1,25};
    encodecctx->width = width;
    encodecctx->height = height;
    encodecctx->bit_rate = bitrate;
    encodecctx->gop_size = 1;
    encodecctx->max_b_frames=1;
    encodecctx->pix_fmt = AV_PIX_FMT_YUV420P;
   // av_opt_set(encodecctx->priv_data, "preset", "slow", 0);
    av_opt_set(encodecctx->priv_data, "tune", "zerolatency", 0);
    got=avcodec_open2(encodecctx,encodec,NULL);
    av_image_alloc(frameyuv->data,frameyuv->linesize,width,height,PIX_FMT_YUV420P,1);
    frameyuv->width=width;
    frameyuv->height=height;
    frameyuv->format=encodecctx->pix_fmt;
    frameyuv->pts=0;
    while(av_read_frame(formatctx,&in_packet)>=0){
        if(addr.size()){
            avcodec_decode_video2(codecctx,frame,&got,&in_packet);
            sws_scale(swsctx,(const uint8_t **)frame->data,frame->linesize,0,height,frameyuv->data,frameyuv->linesize);
            av_init_packet(&out_packet);
            out_packet.data=NULL;
            out_packet.size=0;
            frameyuv->pts++;
            avcodec_encode_video2(encodecctx,&out_packet,frameyuv,&got);
            send(VIDEO,out_packet.data,out_packet.size);
            av_free_packet(&out_packet);
        }
        av_free_packet(&in_packet);
    }
}
void Background::startgame(int index,QString naddr,int nport,QString username)
{
    addr=naddr;
    port=nport;
    QString cmd=QString("sudo -u remote%1 USER=%2 "+shdir+"%3.sh %4").arg(dn).arg(username).arg(index).arg(dn);
    game.start(cmd);
}
void Background::stopgame(){
    //int pid=xdo_get_pid_window(xdo,CURRENTWINDOW);
    //game.start(QString("kill %1").arg(xdo_get_pid_window(xdo,CURRENTWINDOW)));
    QProcess kill;
    kill.start(QString(shdir+"kill.sh %1").arg(dn));
    kill.waitForFinished();
    game.close();
    game.waitForFinished();

    addr="";
}
