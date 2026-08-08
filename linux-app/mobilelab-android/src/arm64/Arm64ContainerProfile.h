#pragma once
#include <QString>
#include <QJsonObject>

struct Arm64ContainerProfile {
    QString name="ampere-arm64";
    QString architecture="aarch64";
    int cpuShares=1024;
    int memoryMiB=4096;
    bool preferNative=true;
    bool requireKvm=false;
    QJsonObject toJson() const{return {{"name",name},{"architecture",architecture},{"cpu_shares",cpuShares},{"memory_mib",memoryMiB},{"prefer_native",preferNative},{"require_kvm",requireKvm}};}
};
