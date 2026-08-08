#pragma once
#include <QObject>
#include <QString>
#include <QStringList>
#include <QJsonObject>

class WaydroidBackend final : public QObject {
    Q_OBJECT
public:
    explicit WaydroidBackend(QObject *parent=nullptr);
    bool available() const;
    bool probe();
    bool init(const QString &systemImage=QString());
    bool start();
    bool stop();
    bool sessionStart();
    bool sessionStop();
    bool shell(const QStringList &args,QString *out=nullptr) const;
    QJsonObject capabilities() const;
    QString lastError() const { return m_error; }

signals:
    void logMessage(const QString &message);

private:
    bool run(const QString &program,const QStringList &args,QString *out=nullptr) const;
    QString m_binary;
    QString m_error;
    bool m_available=false;
    bool m_initialized=false;
};
