#include "MatrixExecutor.h"
#include "AndroidEmulator.h"
#include "ArtifactCollector.h"
#include "ResourceScheduler.h"
#include <QDateTime>
#include <QJsonArray>

MatrixExecutor::MatrixExecutor(AndroidEmulator *e,ResourceScheduler *s,ArtifactCollector *a,QObject *p):QObject(p),m_emulator(e),m_scheduler(s),m_artifacts(a){}

QString MatrixExecutor::run(const QVector<MatrixTarget> &targets){
    const QString id="matrix-"+QDateTime::currentDateTimeUtc().toString("yyyyMMdd-HHmmss");
    const QString dir=m_artifacts->beginRun(id);
    QJsonArray results;
    for(const auto &t:targets){
        emit logMessage("matrix: starting "+t.avd);
        QString job=m_scheduler->enqueue(t.avd,t.command.isEmpty()?"android-test":t.command,1);
        bool started=m_emulator->start(t.avd,true);
        bool ok=started;
        if(started){
            QString out;
            ok=m_emulator->adb({"-s",t.avd,"wait-for-device"},&out);
            m_artifacts->collectLogcat(t.avd,dir+"/"+t.avd);
            m_artifacts->collectScreenshot(t.avd,dir+"/"+t.avd);
            m_emulator->stop(t.avd);
        }
        QJsonObject r{{"avd",t.avd},{"api",t.api},{"abi",t.abi},{"job",job},{"passed",ok}};
        results.append(r);
        emit targetFinished(t.avd,ok);
        emit logMessage("matrix: "+t.avd+(ok?" PASS":" FAIL"));
    }
    m_lastRun={{"id",id},{"artifactDir",dir},{"targets",results}};
    m_artifacts->writeText(dir,"run.json",QJsonDocument(m_lastRun).toJson(QJsonDocument::Indented));
    return id;
}
