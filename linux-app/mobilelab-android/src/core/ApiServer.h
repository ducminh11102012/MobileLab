#pragma once
#include <QObject>
#include <QTcpServer>
#include <QJsonObject>
class AndroidRuntime; class ResourceScheduler;
class ApiServer:public QObject{
 Q_OBJECT
public:ApiServer(AndroidRuntime*,ResourceScheduler*,QObject *parent=nullptr);bool listen(quint16 port=4000);quint16 port()const{return m_server.serverPort();}
signals:void logMessage(const QString&);
private slots:void incoming();
private:QByteArray response(int code,const QJsonObject&body)const;void handle(QTcpSocket*,const QByteArray&);
 QTcpServer m_server;AndroidRuntime*m_runtime;ResourceScheduler*m_scheduler;
};
