#include "ArtifactCollector.h"
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QDateTime>

ArtifactCollector::ArtifactCollector(const QString &root){
    m_root=root.isEmpty()?QDir::currentPath()+"/artifacts":root;
    QDir().mkpath(m_root);
}

QString ArtifactCollector::beginRun(const QString &runId){
    QString safe=runId;
    safe.replace(QRegularExpression("[^A-Za-z0-9_.-]"),"_");
    QString dir=m_root+"/"+safe+"-"+QDateTime::currentDateTimeUtc().toString("yyyyMMdd-HHmmss");
    QDir().mkpath(dir);
    return dir;
}

bool ArtifactCollector::writeText(const QString &runDir,const QString &name,const QString &text){
    QDir().mkpath(runDir);
    QFile f(runDir+"/"+name);
    if(!f.open(QIODevice::WriteOnly|QIODevice::Text)) return false;
    f.write(text.toUtf8());
    return true;
}

bool ArtifactCollector::collectLogcat(const QString &serial,const QString &runDir){
    QProcess p;
    p.start("adb",{"-s",serial,"logcat","-d","-v","threadtime"});
    if(!p.waitForFinished(20000)) return false;
    return writeText(runDir,"logcat.txt",QString::fromUtf8(p.readAllStandardOutput()+p.readAllStandardError()));
}

bool ArtifactCollector::collectScreenshot(const QString &serial,const QString &runDir){
    QProcess p;
    p.start("adb",{"-s",serial,"exec-out","screencap","-p"});
    if(!p.waitForFinished(15000)) return false;
    QFile f(runDir+"/screenshot.png");
    if(!f.open(QIODevice::WriteOnly)) return false;
    f.write(p.readAllStandardOutput());
    return true;
}
