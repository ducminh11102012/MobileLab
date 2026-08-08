#include "ApiServer.h"
#include "AndroidRuntime.h"
#include "ResourceScheduler.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
ApiServer::ApiServer(AndroidRuntime*r,ResourceScheduler*s,QObject*p):QObject(p),m_runtime(r),m_scheduler(s){connect(&m_server,&QTcpServer::newConnection,this,&ApiServer::incoming);}
bool ApiServer::listen(quint16 p){return m_server.listen(QHostAddress::LocalHost,p);}
QByteArray ApiServer::response(int code,const QJsonObject&body)const{const auto b=QJsonDocument(body).toJson(QJsonDocument::Compact);const QByteArray reason=code==200?"OK":code==404?"Not Found":"Bad Request";return QByteArray("HTTP/1.1 ")+QByteArray::number(code)+" "+reason+"\r\nContent-Type: application/json\r\nContent-Length: "+QByteArray::number(b.size())+"\r\nConnection: close\r\n\r\n"+b;}
void ApiServer::incoming(){while(m_server.hasPendingConnections()){auto*s=m_server.nextPendingConnection();connect(s,&QTcpSocket::readyRead,this,[this,s]{handle(s,s->readAll());});}}
void ApiServer::handle(QTcpSocket*s,const QByteArray&raw){const auto first=raw.split('\n').value(0).trimmed(),parts=first.split(' ');if(parts.size()<2){s->write(response(400,{{"error","invalid request"}}));s->disconnectFromHost();return;}const QString method=parts[0],path=parts[1];QJsonObject out;int code=200;
if(method=="GET"&&path=="/status")out={{"runtime",m_runtime->status()},{"scheduler",m_scheduler->status()}};
else if(method=="GET"&&path=="/devices"){QJsonArray a;for(const auto&t:m_runtime->targets())a.append(QJsonObject{{"id",t.id},{"api",t.api},{"arch",t.arch},{"state",t.state},{"pid",t.pid}});out={{"devices",a}};}
else if(method=="POST"&&path.startsWith("/devices/")&&path.endsWith("/start")){out={{"ok",m_runtime->start(path.section('/',2,2))}};}
else if(method=="POST"&&path.startsWith("/devices/")&&path.endsWith("/stop")){out={{"ok",m_runtime->stop(path.section('/',2,2))}};}
else if(method=="POST"&&path=="/runs"){out={{"id",m_scheduler->enqueue("arm64-dev","android-test",1)}};}
else{code=404;out={{"error","not found"}};}
s->write(response(code,out));s->disconnectFromHost();emit logMessage(method+" "+path);}
