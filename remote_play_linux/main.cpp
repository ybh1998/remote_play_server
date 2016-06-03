#include <QCoreApplication>
#include <QCryptographicHash>
#include "defines.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    av_register_all();
    avdevice_register_all();
    Server server;
    server.utp["wyh"]=QString(
        QCryptographicHash::hash ( "wyh", QCryptographicHash::Md5 ));
    server.utp["sx"]=QString(
        QCryptographicHash::hash ( "sx", QCryptographicHash::Md5 ));
    server.utp["nannan"]=QString(
        QCryptographicHash::hash ( "nannan", QCryptographicHash::Md5 ));
    server.utp["wyh"]=QString(
        QCryptographicHash::hash ( "wyh", QCryptographicHash::Md5 ));
    server.utp["ybh1998"]=QString(
        QCryptographicHash::hash ( "112358", QCryptographicHash::Md5 ));
    return a.exec();
}
