#pragma once
#include <QObject>
#include <QStringList>
#include <QJsonObject>

struct MatrixTarget { QString avd; QString api; QString abi="arm64-v8a"; QString command; };

class AndroidEmulator;
class ResourceScheduler;
class ArtifactCollector;

class MatrixExecutor final : public QObject {
    Q_OBJECT
public:
    MatrixExecutor(AndroidEmulator *emulator, ResourceScheduler *scheduler, ArtifactCollector *artifacts, QObject *parent=nullptr);
    QString run(const QVector<MatrixTarget> &targets);
    QJsonObject lastRun() const { return m_lastRun; }
signals:
    void logMessage(const QString &);
    void targetFinished(const QString &, bool);
private:
    AndroidEmulator *m_emulator;
    ResourceScheduler *m_scheduler;
    ArtifactCollector *m_artifacts;
    QJsonObject m_lastRun;
};
