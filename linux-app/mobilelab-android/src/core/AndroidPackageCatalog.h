#pragma once
#include <QString>
#include <QVector>

struct AndroidPackage {
    QString id;
    QString displayName;
    QString api;
    QString abi;
    bool installed=false;
};

class AndroidPackageCatalog {
public:
    explicit AndroidPackageCatalog(const QString &sdkRoot = {});
    QVector<AndroidPackage> systemImages() const;
    QVector<AndroidPackage> installedPackages() const;
    bool hasPackage(const QString &id) const;
    QString sdkRoot() const { return m_sdkRoot; }
private:
    QString m_sdkRoot;
};
