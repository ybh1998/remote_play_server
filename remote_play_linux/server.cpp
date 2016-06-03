#include "server.h"


Server::Server()
{
    bind(::port);
    for(int i=1;i<=backcount;i++){
        ava.insert(new Background(this,i));
    }
    connect(this,&Server::readyRead,this,&Server::receive);
    connect(&timer,&QTimer::timeout,[=](){
        for( map< pair<QString,quint16> , Info * >::iterator i=info.begin();i!=info.end();){
            if(time(NULL) - (*i).second->last > timeout){
                qDebug() << QString("User %1 disconnected from %2:%3")
                            .arg((*i).second->username).arg((*i).first.first).arg((*i).first.second);
                if((*i).second->server){
                    (*i).second->server->stopgame();
                    ava.insert((*i).second->server);
                }
                if((*i).second->username.size()){
                    uti.erase((*i).second->username);
                }
                delete (*i).second;
                info.erase(i++);
            }else
                i++;
        }
    });
    timer.setInterval(timeout*1000);
    timer.start();
}
Server::~Server(){
    for(set< Background * >::iterator i=ava.begin();i!=ava.end();i++){
        delete *i;
    }
}

void Server::send(int id, uint8_t *data, int size){
    uint8_t *tmpdata = (uint8_t *)malloc(size+4);
    *((int *)tmpdata)=id;
    memcpy((void *)(tmpdata+4),(void *)data,size);
    writeDatagram((char *)tmpdata,size+4,addr,port);
}

void Server::receive(){
    uint8_t *data = (uint8_t *)malloc(buffer);
    int id,size;
    while(hasPendingDatagrams()){
        memset((void *)data,0,buffer);
        size = readDatagram((char *)data,buffer,&addr,&port);
        id=(int)(*data);
        data+=4;
        if(info.count(pair<QString,quint16>(addr.toString(),port))==0){
            qDebug() << QString("New connection from %1:%2").arg(addr.toString()).arg(port);
            info[pair<QString,quint16>(addr.toString(),port)]=new Info;
            info[pair<QString,quint16>(addr.toString(),port)]->conn=pair<QString,quint16>(addr.toString(),port);
        }
        info[pair<QString,quint16>(addr.toString(),port)]->last=time(NULL);
        switch (id) {
        case KEEPALIVE:
            break;
        case UPDATE:
            send(UPDATE,(uint8_t *)ver,sizeof(ver));
            break;
        case LOGIN:{
            QString tmpstr((char *)data);
            QStringList tmplst=tmpstr.split(" ");
            QString username=tmplst[0],password=tmplst[1];
            if(utp.count(username))
                if(utp[username]==password){
                    if(uti.count(username)){
                        Info *cinfo=uti[username];
                        if(cinfo->server){
                            cinfo->server->stopgame();
                            ava.insert(cinfo->server);
                        }
                        uti.erase(username);
                        info.erase(cinfo->conn);
                        delete cinfo;
                    }
                    uti[username]=info[pair<QString,quint16>(addr.toString(),port)];
                    info[pair<QString,quint16>(addr.toString(),port)]->username=username;
                    qDebug() << QString("%1 loged in from %2:%3").arg(username).arg(addr.toString()).arg(port);
                    send(LOGIN,(uint8_t *)&(LOGIN_SUCCESS),sizeof(int));
                }else
                    send(LOGIN,(uint8_t *)&(LOGIN_PWDERROR),sizeof(int));
            else
                send(LOGIN,(uint8_t *)&(LOGIN_NOUSER),sizeof(int));
            break;
        }
        case START:{
            if( info[pair<QString,quint16>(addr.toString(),port)]->username.size()==0)
                break;
            if(ava.size()!=0){
                Background *server=*(ava.begin());
                ava.erase(ava.begin());
                info[pair<QString,quint16>(addr.toString(),port)]->server=server;
                server->startgame((int)(*data),addr.toString(),port,info[pair<QString,quint16>(addr.toString(),port)]->username);
                send(START,NULL,0);
            }
            break;
        }
        case STOP:{
            Background *server = info[pair<QString,quint16>(addr.toString(),port)]->server;
            if(server){
                server->stopgame();
                ava.insert(server);
                info[pair<QString,quint16>(addr.toString(),port)]->server = NULL;
            }
            break;
        }
        case MOUSE_MOVE:{
            Background *server = info[pair<QString,quint16>(addr.toString(),port)]->server;
            if(server){
                server->mouse_move(((int *)data)[0],((int *)data)[1]);
            }
            break;
        }
        case MOUSE_DOWN:{
            Background *server = info[pair<QString,quint16>(addr.toString(),port)]->server;
            if(server){
                server->mouse_down((int)(*data));
            }
            break;
        }
        case MOUSE_UP:{
            Background *server = info[pair<QString,quint16>(addr.toString(),port)]->server;
            if(server){
                server->mouse_up((int)(*data));
            }
            break;
        }
        case KEY_DOWN:{
            Background *server = info[pair<QString,quint16>(addr.toString(),port)]->server;
            if(server){
                server->key_down(((char *)data));
            }
            break;
        }
        case KEY_UP:{
            Background *server = info[pair<QString,quint16>(addr.toString(),port)]->server;
            if(server){
                server->key_up(((char *)data));
            }
            break;
        }
        case TYPE:{
            Background *server = info[pair<QString,quint16>(addr.toString(),port)]->server;
            if(server){
                server->type(((char *)data));
            }
            break;
        }
        default:
            break;
        }
        data-=4;
    }
    free(data);
}
