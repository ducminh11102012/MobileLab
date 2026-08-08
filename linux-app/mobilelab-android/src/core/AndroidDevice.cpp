#include "AndroidDevice.h"
#include "AndroidEmulator.h"
#include <QProcess>
#include <QDir>

static QString adbBin(){ QString s=qEnvironmentVariable("ANDROID_HOME"); if(s.isEmpty()) s=qEnvironmentVariable("ANDROID_SDK_ROOT"); if(s.isEmpty()) s=QDir::homePath()+"/Android/Sdk"; return s+"/platform-tools/adb"; }
QVector<AndroidDeviceInfo> AndroidDeviceManager::list() const{
    QVector<AndroidDeviceInfo> r; QProcess p; p.start(adbBin(),{"devices","-l"}); if(!p.waitForFinished(5000)) return r;
    const auto lines=QString::fromLocal8Bit(p.readAllStandardOutput()).split('\n',Qt::SkipEmptyParts);
    for(int i=1;i<lines.size();++i){ auto x=lines[i].simplified().split(' '); if(x.size()<2) continue; AndroidDeviceInfo d; d.serial=x[0]; d.state=x[1]; for(auto &v:x) if(v.startsWith("model:")) d.model=v.mid(6).replace('_',' '); r.push_back(d); }
    return r;
}
bool AndroidDeviceManager::install(const QString &serial,const QString &apk) const{ QProcess p; p.start(adbBin(),{"-s",serial,"install","-r",apk}); return p.waitForFinished(120000)&&p.exitCode()==0; }
bool AndroidDeviceManager::shell(const QString &serial,const QStringList &args,QString *out) const{ QProcess p; QStringList a={"-s",serial,"shell"}; a+=args; p.start(adbBin(),a); if(!p.waitForFinished(15000)) return false; if(out) *out=QString::fromLocal8Bit(p.readAllStandardOutput()+p.readAllStandardError()); return p.exitCode()==0; }
