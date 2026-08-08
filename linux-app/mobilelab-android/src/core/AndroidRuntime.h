#pragma once
#include <QObject>
#include <QProcess>
#include <QStringList>
#include <QJsonObject>

struct AndroidTarget {
    QString id;
    QString api;
    QString arch;
    QString state = "stopped";
    int pid = -1;
};

class AndroidRuntime : public QObject {
    Q_OBJECT
public:
    explicit AndroidRuntime(QObject *parent = nullptr);
    bool probe();
    void refreshTargets();
    bool start(const QString &id);
    bool stop(const QString &id);
    bool restart(const QString &id);
    bool shell(const QString &id, const QString &command);
    bool screenshot(const QString &id, const QString &path);
    QJsonObject status() const;
    bool kvmAvailable() const { return m_kvm; }
    bool qemuAvailable() const { return m_qemu; }
    bool emulatorAvailable() const { return m_emulator; }
    QString architecture() const { return m_arch; }
    QString kernel() const { return m_kernel; }
    QList<AndroidTarget> targets() const { return m_targets; }
signals:
    void logMessage(const QString &message);
    void targetChanged(const QString &id);
private:
    int indexOf(const QString &id) const;
    QString findExecutable(const QStringList &names) const;
    QList<AndroidTarget> m_targets;
    QString m_arch, m_kernel;
    bool m_kvm=false, m_qemu=false, m_emulator=false;
};
