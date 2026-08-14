#pragma once
#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <QVector>
#include <QHash>

struct RunRequest {
    QString id;
    int cost=1;
    int priority=0;
    int maxRetries=0;
    int attempt=0;
    QString target;
    QString command;
};

class ResourceScheduler:public QObject{
 Q_OBJECT
public:
 explicit ResourceScheduler(QObject* p=nullptr);
 void configure(int cpu,int memoryMb);
 QString enqueue(const QString&,const QString&,int cost=1);
 QString submit(const QString&,const QString&,int cost,int priority,int maxRetries);
 void cancel(const QString&);
 QJsonObject dryRun(const QString&,const QString&,int cost,int priority=0)const;
 QJsonObject status()const;
 QStringList logs()const{return m_logs;}
signals:void logMessage(const QString&);void jobChanged();
private slots:void tick();
private:
 int capacity()const{return m_cpu*2;}
 int nextRunnableIndex()const;
 int m_cpu=1,m_memory=2048,m_used=0;quint64 m_seq=0;
 QVector<RunRequest> m_queue;
 QHash<QString,RunRequest> m_running;
 QStringList m_logs;
 QTimer m_timer;
};
