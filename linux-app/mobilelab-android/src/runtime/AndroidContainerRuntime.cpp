#include "AndroidContainerRuntime.h"
#include <QJsonObject>

AndroidContainerRuntime::AndroidContainerRuntime(QObject *parent):QObject(parent),m_waydroid(this){
    connect(&m_waydroid,&WaydroidBackend::logMessage,this,&AndroidContainerRuntime::logMessage);
}

bool AndroidContainerRuntime::probe(){
    emit logMessage("MobileLab Android: probing ARM64 container runtime");
    return m_waydroid.probe()||!m_waydroid.available();
}

bool AndroidContainerRuntime::start(){
    if(!m_waydroid.available()) return false;
    if(!m_waydroid.start()) return false;
    return m_waydroid.sessionStart();
}

bool AndroidContainerRuntime::stop(){
    if(!m_waydroid.available()) return false;
    m_waydroid.sessionStop();
    return m_waydroid.stop();
}

QJsonObject AndroidContainerRuntime::diagnostics() const{
    return {{"arm64",m_arm.probe()},{"waydroid",m_waydroid.capabilities()},{"nested_virtualization_required",false}};
}
