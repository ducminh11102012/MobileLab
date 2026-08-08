#include "WaydroidImageCatalog.h"
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>

QVector<AndroidImage> WaydroidImageCatalog::discover() const{
    QVector<AndroidImage> r;
    const QStringList roots={QDir::homePath()+"/.local/share/waydroid/images", "/usr/share/waydroid-extra/images", "/var/lib/waydroid/images"};
    for(const auto &root:roots){
        QDir d(root);
        if(!d.exists()) continue;
        AndroidImage i; i.name=QFileInfo(root).absoluteFilePath(); i.path=root; i.architecture="arm64-or-host";
        i.valid=d.exists("system.img")&&d.exists("vendor.img"); r.push_back(i);
    }
    return r;
}

QJsonArray WaydroidImageCatalog::toJson() const{
    QJsonArray a;
    for(const auto &i:discover()) a.append(QJsonObject{{"name",i.name},{"path",i.path},{"architecture",i.architecture},{"valid",i.valid}});
    return a;
}
