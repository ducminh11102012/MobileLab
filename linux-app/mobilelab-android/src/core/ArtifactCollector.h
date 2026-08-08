#pragma once
#include <QString>
#include <QStringList>

class ArtifactCollector {
public:
    explicit ArtifactCollector(const QString &root = {});
    QString beginRun(const QString &runId);
    bool collectLogcat(const QString &serial, const QString &runDir);
    bool collectScreenshot(const QString &serial, const QString &runDir);
    bool writeText(const QString &runDir, const QString &name, const QString &text);
    QString root() const { return m_root; }
private:
    QString m_root;
};
