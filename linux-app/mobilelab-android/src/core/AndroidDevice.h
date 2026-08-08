#pragma once
#include <QString>
#include <QVector>

struct AndroidDeviceInfo { QString serial,model,version,abi,state; };
class AndroidDeviceManager {
public:
    QVector<AndroidDeviceInfo> list() const;
    bool install(const QString &serial,const QString &apk) const;
    bool shell(const QString &serial,const QStringList &args,QString *out=nullptr) const;
};
