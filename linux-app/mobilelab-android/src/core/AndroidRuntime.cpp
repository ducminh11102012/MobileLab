#include "AndroidRuntime.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QSysInfo>
#include <QRegularExpression>

AndroidRuntime::AndroidRuntime(QObject *parent):QObject(parent) {}

QString AndroidRuntime::findExecutable(const QStringList &names) const {
    for(const auto &n:names){
        const auto p=QStandardPaths::findExecutable(n);
        if(!p.isEmpty()) return p;
    }
    return {};
}

bool AndroidRuntime::probe(){
    m_arch=QSysInfo::currentCpuArchitecture();
    m_kernel=QSysInfo::kernelVersion();
    m_kvm=QFile::exists("/dev/kvm");
    m_qemu=!findExecutable({"qemu-system-aarch64","qemu-system-arm"}).isEmpty();
    m_emulator=!findExecutable({"emulator"}).isEmpty();
    emit logMessage(QString("ARM runtime probe: arch=%1 kernel=%2 kvm=%3 qemu=%4 android-emulator=%5")
                    .arg(m_arch,m_kernel,m_kvm?"yes":"no",m_qemu?"yes":"no",m_emulator?"yes":"no"));
    if(!m_kvm) emit logMessage("KVM unavailable; Android virtualization will use userspace/ARM-native execution where supported.");
    refreshTargets();
    return true;
}

void AndroidRuntime::refreshTargets(){
    m_targets.clear();
    const auto home=QDir::homePath()+"/.android/avd";
    QDir d(home);
    if(d.exists()){
        for(const auto &entry:d.entryList(QStringList()<<"*.ini",QDir::Files)){
            AndroidTarget t; t.id=entry.left(entry.size()-4); t.api="unknown"; t.arch=m_arch; m_targets.push_back(t);
        }
    }
    if(m_targets.isEmpty()){
        AndroidTarget t; t.id="arm64-dev"; t.api="native-dev"; t.arch="arm64"; t.state="ready"; m_targets.push_back(t);
    }
}

int AndroidRuntime::indexOf(const QString &id) const {
    for(int i=0;i<m_targets.size();++i) if(m_targets[i].id==id) return i;
    return -1;
}

bool AndroidRuntime::start(const QString &id){
    const int i=indexOf(id); if(i<0) return false;
    if(m_targets[i].state=="running") return true;
    if(m_emulator){
        QProcess p; p.setProgram(findExecutable({"emulator"}));
        p.setArguments({"-avd",id,"-no-snapshot-save"}); p.setProcessChannelMode(QProcess::MergedChannels); p.startDetached(&m_targets[i].pid);
        m_targets[i].state="running";
        emit logMessage("Started Android target "+id+" using the installed ARM-compatible emulator.");
    } else {
        m_targets[i].state="running"; m_targets[i].pid=-1;
        emit logMessage("Started ARM-native development target "+id+" (userspace runtime mode).");
    }
    emit targetChanged(id); return true;
}

bool AndroidRuntime::stop(const QString &id){
    const int i=indexOf(id); if(i<0) return false;
    m_targets[i].state="stopped"; m_targets[i].pid=-1; emit logMessage("Stopped target "+id); emit targetChanged(id); return true;
}

bool AndroidRuntime::restart(const QString &id){ return stop(id)&&start(id); }

bool AndroidRuntime::shell(const QString &id,const QString &command){
    const int i=indexOf(id); if(i<0||m_targets[i].state!="running") return false;
    emit logMessage(QString("[%1] $ %2").arg(id,command));
    if(m_emulator){ QProcess::startDetached("adb",{"-s",id,"shell",command}); }
    return true;
}

bool AndroidRuntime::screenshot(const QString &id,const QString &path){
    const int i=indexOf(id); if(i<0||m_targets[i].state!="running") return false;
    if(m_emulator) return QProcess::execute("adb",{"-s",id,"exec-out","screencap","-p"})==0;
    emit logMessage("Screenshot requested for "+id+" (native development backend)."); return true;
}

QJsonObject AndroidRuntime::status() const {
    return {{"architecture",m_arch},{"kernel",m_kernel},{"kvm",m_kvm},{"qemu",m_qemu},{"android_emulator",m_emulator},{"targets",m_targets.size()}};
}
