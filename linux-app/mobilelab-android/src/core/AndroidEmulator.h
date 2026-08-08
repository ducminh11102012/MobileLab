#pragma once
#include <QString>
#include <QStringList>
#include <QVector>

struct AndroidAvd {
    QString name;
    QString api;
    QString abi;
    QString path;
    bool running=false;
};

struct AndroidEmulatorInfo {
    QString sdkRoot;
    QString emulatorPath;
    QString adbPath;
    QString cmdlineToolsPath;
    bool available=false;
};

class AndroidEmulator {
public:
    AndroidEmulator();
    bool discover();
    bool createAvd(const QString &name,const QString &packageName,const QString &device);
    bool start(const QString &name,bool noWindow=true);
    bool stop(const QString &name);
    bool adb(const QStringList &args,QString *output=nullptr);
    QStringList installedSystemImages() const;
    AndroidEmulatorInfo info() const;
    QVector<AndroidAvd> avds() const { return m_avds; }
    QString sdkRoot() const { return m_sdkRoot; }
    QString emulatorPath() const { return m_emulator; }
    QString adbPath() const { return m_adb; }
private:
    QString m_sdkRoot,m_emulator,m_adb,m_avdManager;
    QVector<AndroidAvd> m_avds;
    bool run(const QString &program,const QStringList &args,QString *output=nullptr,int timeout=10000) const;
};
