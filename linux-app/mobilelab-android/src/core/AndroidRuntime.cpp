#include "AndroidRuntime.h"
#include "AndroidEmulator.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QSysInfo>

AndroidRuntime::AndroidRuntime(QObject *parent):QObject(parent) {}

void AndroidRuntime::setEmulator(AndroidEmulator *emulator){ m_androidEmulator=emulator; }

QString AndroidRuntime::findExecutable(const QStringList &names) const{
    for(const auto &n:names){const auto p=QStandardPaths::findExecutable(n);if(!p.isEmpty())return p;}
    return {};
}

bool AndroidRuntime::probe(){
    m_arch=QSysInfo::currentCpuArchitecture(); m_kernel=QSysInfo::kernelVersion(); m_kvm=QFile::exists("/dev/kvm");
    m_qemu=!findExecutable({"qemu-system-aarch64","qemu-system-arm"}).isEmpty();
    m_emulator=m_androidEmulator?m_androidEmulator->info().available:!findExecutable({"emulator"}).isEmpty();
    emit logMessage(QString("ARM runtime probe: arch=%1 kernel=%2 kvm=%3 qemu=%4 android-emulator=%5").arg(m_arch,m_kernel,m_kvm?"yes":"no",m_qemu?"yes":"no",m_emulator?"yes":"no"));
    if(!m_kvm)emit logMessage("KVM unavailable; workloads requiring nested virtualization are excluded from the execution path.");
    refreshTargets(); return true;
}

void AndroidRuntime::refreshTargets(){
    m_targets.clear();
    if(m_androidEmulator){
        m_androidEmulator->discover();
        for(const auto &a:m_androidEmulator->avds()){
            AndroidTarget t;t.id=a.name;t.api=a.api;t.arch=a.abi;t.state=a.running?"running":"stopped";m_targets.push_back(t);
        }
    }
    if(m_targets.isEmpty()){
        const auto home=QDir::homePath()+"/.android/avd";QDir d(home);
        for(const auto &entry:d.entryList(QStringList()<<"*.ini",QDir::Files)){AndroidTarget t;t.id=entry.left(entry.size()-4);t.api="unknown";t.arch=m_arch;t.state="stopped";m_targets.push_back(t);}
    }
    if(m_targets.isEmpty()){AndroidTarget t;t.id="arm64-dev";t.api="native-dev";t.arch="arm64-v8a";t.state="ready";m_targets.push_back(t);}
}

int AndroidRuntime::indexOf(const QString &id) const{for(int i=0;i<m_targets.size();++i)if(m_targets[i].id==id)return i;return -1;}

bool AndroidRuntime::start(const QString &id){
    const int i=indexOf(id);if(i<0)return false;if(m_targets[i].state=="running")return true;
    bool ok=m_androidEmulator?m_androidEmulator->start(id,true):false;
    if(!ok&&!m_androidEmulator){m_targets[i].state="ready";emit logMessage("ARM-native development target selected: "+id);ok=true;}
    if(ok){m_targets[i].state="running";emit logMessage("Started Android target "+id);emit targetChanged(id);}return ok;
}

bool AndroidRuntime::stop(const QString &id){const int i=indexOf(id);if(i<0)return false;bool ok=m_androidEmulator?m_androidEmulator->stop(id):true;m_targets[i].state="stopped";emit logMessage("Stopped target "+id);emit targetChanged(id);return ok;}
bool AndroidRuntime::restart(const QString &id){return stop(id)&&start(id);}

bool AndroidRuntime::shell(const QString &id,const QString &command){const int i=indexOf(id);if(i<0||m_targets[i].state!="running")return false;if(m_androidEmulator){QString out;const bool ok=m_androidEmulator->adb({"-s",id,"shell",command},&out);emit logMessage("["+id+"] "+out.trimmed());return ok;}return false;}
bool AndroidRuntime::screenshot(const QString &id,const QString &path){const int i=indexOf(id);if(i<0||m_targets[i].state!="running")return false;if(m_androidEmulator){QProcess p;p.start(m_androidEmulator->adbPath(),{"-s",id,"exec-out","screencap","-p"});if(!p.waitForFinished(15000))return false;QFile f(path);if(!f.open(QIODevice::WriteOnly))return false;f.write(p.readAllStandardOutput());return true;}return false;}

QJsonObject AndroidRuntime::status() const{return {{"architecture",m_arch},{"kernel",m_kernel},{"kvm",m_kvm},{"qemu",m_qemu},{"android_emulator",m_emulator},{"targets",m_targets.size()}};}
