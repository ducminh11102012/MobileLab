#pragma once
#include <QString>
#include <QStringList>
class AndroidTestRunner {
public:
    bool connected(const QString &serial) const;
    bool runInstrumentation(const QString &serial,const QString &packageName,const QString &runner,const QStringList &args,QString *output=nullptr) const;
    bool runShellTest(const QString &serial,const QString &command,QString *output=nullptr) const;
};
