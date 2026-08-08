#include "AndroidContainerRuntime.h"
#include <QJsonObject>

AndroidContainerRuntime::AndroidContainerRuntime(QObject *parent):QObject(parent),m_waydroid(this){
    connect(&m_waydroid,&WaydroidBackend::logMessage,this,&AndroidContainerRuntime::logMessage);
}

bool AndroidContainerRuntime::probe(){
    const auto caps=m_arm.probe();
    const bool arm64=caps.value("aarch64").toBool();
    const bool viable=caps.value("container_backend_viable").toBool();

    emit logMessage(QString("MobileLab Android: ARM64=%1 container-primitives=%2")
        .arg(arm64?"yes":"no")
        .arg(viable?"ready":"incomplete"));

    if(!arm64||!viable) return false;
    return m_waydroid.probe();
}

bool AndroidContainerRuntime::start(){
    const auto caps=m_arm.probe();
    if(!caps.value("container_backend_viable").toBool()){
        emit logMessage("Android container start refused: host capabilities are incomplete");
        return false;
    }
    if(!m_waydroid.available()){
        emit logMessage("Android container start refused: Waydroid is not installed");
        return false;
    }
    if(!m_waydroid.start()) return false;
    return m_waydroid.sessionStart();
}

bool AndroidContainerRuntime::stop(){
    if(!m_waydroid.available()) return false;
    const bool session=m_waydroid.sessionStop();
    const bool container=m_waydroid.stop();
    return session&&container;
}

QJsonObject AndroidContainerRuntime::diagnostics() const{
    return {
        {"arm64",m_arm.probe()},
        {"waydroid",m_waydroid.capabilities()},
        {"nested_virtualization_required",false}
    };
}
