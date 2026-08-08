#pragma once
#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <QQueue>
struct RunRequest{QString id;int cost=1;QString target;QString command;};
class ResourceScheduler:public QObject{
 Q_OBJECT
public: explicit ResourceScheduler(QObject* p=nullptr); void configure(int cpu,int memoryMb); QString enqueue(const QString&,const QString&,int cost=1); void cancel(const QString&); QJsonObject status()const; QStringList logs()const{return m_logs;}
signals:void logMessage(const QString&);void jobChanged();
private slots:void tick();
private:int m_cpu=1,m_memory=2048,m_used=0;quint64 m_seq=0;QQueue<RunRequest> m_queue;QHash<QString,RunRequest> m_running;QStringList m_logs;QTimer m_timer;
};
