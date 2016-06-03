#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QProcess>
#include <QThread>
#include <QTimer>
#include "defines.h"
#include <set>
#include <map>
#include <QEventLoop>
#include "background.h"
using namespace std;

class Background;

class Server : public QUdpSocket
{
    Q_OBJECT
public:
    struct Info{
        pair<QString,quint16> conn;
        QString username;
        time_t last;
        Background *server=NULL;
        QProcess run;
    };
    set< Background * > ava;
    map< QString,QString > utp;
    map< QString,Info * > uti;
    map< pair<QString,quint16> , Info * > info;
    Server();
    ~Server();
    void add();
    void receive();
    void send(int id,uint8_t *data,int size);
private:
    int dni;
    QTimer timer;
    QProcess process[server_num];
    QHostAddress addr;
    quint16 port;
    QEventLoop loop;
signals:
    void newback();
};

#endif // SERVER_H
