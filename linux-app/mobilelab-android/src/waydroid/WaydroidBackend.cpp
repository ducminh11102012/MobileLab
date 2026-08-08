#include "WaydroidBackend.h"
#include <QProcess>
#include <QStandardPaths>
#include <QSysInfo>

WaydroidBackend::WaydroidBackend(QObject *parent):QObject(parent){
    m_binary=QStandardPaths::findExecutable("waydroid");
    m_available=!m_binary.isEmpty();
}

bool WaydroidBackend::available() const{return m_available;}

bool WaydroidBackend::run(const QString &program,const QStringList &args,QString *out) const{
    QProcess p;
    p.start(program,args);
    if(!p.waitForFinished(30000)) return false;
    if(out) *out=QString::fromUtf8(p.readAllStandardOutput()+p.readAllStandardError());
    return p.exitStatus()==QProcess::NormalExit&&p.exitCode()==0;
}

bool WaydroidBackend::probe(){
    if(!m_available){m_error="waydroid executable was not found";emit logMessage(m_error);return false;}
    QString out;
    const bool ok=run(m_binary,{"status"},&out);
    emit logMessage(QString("Waydroid probe: arch=%1 available=yes").arg(QSysInfo::currentCpuArchitecture()));
    if(!out.isEmpty()) emit logMessage(out.trimmed());
    return ok;
}

bool WaydroidBackend::init(const QString &systemImage){
    if(!m_available) return false;
    QStringList a={"init"};
    if(!systemImage.isEmpty()) a<<"-i"<<systemImage;
    QString out;
    const bool ok=run(m_binary,a,&out);
    if(ok) m_initialized=true; else m_error=out.trimmed();
    emit logMessage(ok?"Waydroid container initialized":"Waydroid initialization failed: "+m_error);
    return ok;
}

bool WaydroidBackend::start(){
    if(!m_available) return false;
    QString out; const bool ok=run(m_binary,{"container","start"},&out);
    emit logMessage(ok?"Waydroid container started":"Waydroid container start failed: "+out.trimmed());
    return ok;
}

bool WaydroidBackend::stop(){
    if(!m_available) return false;
    QString out; const bool ok=run(m_binary,{"container","stop"},&out);
    emit logMessage(ok?"Waydroid container stopped":"Waydroid container stop failed: "+out.trimmed());
    return ok;
}

bool WaydroidBackend::sessionStart(){
    if(!m_available) return false;
    QString out; const bool ok=run(m_binary,{"session","start"},&out);
    emit logMessage(ok?"Waydroid session started":"Waydroid session start failed: "+out.trimmed());
    return ok;
}

bool WaydroidBackend::sessionStop(){
    if(!m_available) return false;
    QString out; const bool ok=run(m_binary,{"session","stop"},&out);
    emit logMessage(ok?"Waydroid session stopped":"Waydroid session stop failed: "+out.trimmed());
    return ok;
}

bool WaydroidBackend::shell(const QStringList &args,QString *out) const{
    if(!m_available) return false;
    return run(m_binary,QStringList{"shell"}+args,out);
}

QJsonObject WaydroidBackend::capabilities() const{
    return {{"backend","waydroid"},{"available",m_available},{"initialized",m_initialized},{"architecture",QSysInfo::currentCpuArchitecture()},{"containerized",true},{"nested_virtualization_required",false}};
}
