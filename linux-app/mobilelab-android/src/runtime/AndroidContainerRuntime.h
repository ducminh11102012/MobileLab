#pragma once
#include <QObject>
#include <QJsonObject>
#include "../waydroid/WaydroidBackend.h"
#include "../arm64/Arm64CapabilityProbe.h"

class AndroidContainerRuntime final : public QObject {
    Q_OBJECT
public:
    explicit AndroidContainerRuntime(QObject *parent=nullptr);
    bool probe();
    bool start();
    bool stop();
    QJsonObject diagnostics() const;
signals:
    void logMessage(const QString &message);
private:
    WaydroidBackend m_waydroid;
    Arm64CapabilityProbe m_arm;
};
