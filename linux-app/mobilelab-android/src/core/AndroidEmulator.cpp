#include "AndroidEmulator.h"
#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

AndroidEmulator::AndroidEmulator(){
    m_sdkRoot=qEnvironmentVariable("ANDROID_HOME");
    if(m_sdkRoot.isEmpty()) m_sdkRoot=qEnvironmentVariable("ANDROID_SDK_ROOT");
    if(m_sdkRoot.isEmpty()) m_sdkRoot=QDir::homePath()+"/Android/Sdk";
    m_emulator=m_sdkRoot+"/emulator/emulator";
    m_adb=m_sdkRoot+"/platform-tools/adb";
}

bool AndroidEmulator::run(const QString &program,const QStringList &args,QString *output,int timeout) const{
    QProcess p; p.start(program,args);
    if(!p.waitForStarted(3000)||!p.waitForFinished(timeout)) return false;
    if(output) *output=QString::fromLocal8Bit(p.readAllStandardOutput()+p.readAllStandardError());
    return p.exitStatus()==QProcess::NormalExit&&p.exitCode()==0;
}

bool AndroidEmulator::discover(){
    m_avds.clear(); QString out;
    if(!run(m_emulator,{"-list-avds"},&out)) return false;
    for(const auto &line:out.split('\n',Qt::SkipEmptyParts)){
        AndroidAvd a; a.name=line.trimmed(); a.abi="arm64-v8a"; a.api="unknown";
        m_avds.push_back(a);
    }
    return true;
}

bool AndroidEmulator::createAvd(const QString &name,const QString &packageName,const QString &device){
    QString sdk=m_sdkRoot+"/cmdline-tools/latest/bin/avdmanager";
    if(!QFileInfo::exists(sdk)) sdk=m_sdkRoot+"/cmdline-tools/bin/avdmanager";
    return run(sdk,{"create","avd","-n",name,"-k",packageName,"-d",device,"--force"},nullptr,30000);
}

bool AndroidEmulator::start(const QString &name,bool noWindow){
    if(!QFileInfo::exists(m_emulator)) return false;
    QStringList a={"-avd",name,"-no-boot-anim","-no-snapshot"};
    if(noWindow) a<<"-no-window";
    QProcess *p=new QProcess(); p->setProgram(m_emulator); p->setArguments(a); p->setProcessChannelMode(QProcess::MergedChannels); p->start();
    return p->waitForStarted(3000);
}

bool AndroidEmulator::stop(const QString &name){
    QString out; return adb({"-s",name,"emu","kill"},&out);
}

bool AndroidEmulator::adb(const QStringList &args,QString *output){ return run(m_adb,args,output,15000); }
