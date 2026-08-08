#include "AndroidTestRunner.h"
#include "AndroidDevice.h"
bool AndroidTestRunner::connected(const QString &serial) const { AndroidDeviceManager m; for(auto &d:m.list()) if(d.serial==serial&&d.state=="device") return true; return false; }
bool AndroidTestRunner::runInstrumentation(const QString &serial,const QString &pkg,const QString &runner,const QStringList &args,QString *out) const { AndroidDeviceManager m; QStringList a={"am","instrument","-w","-r"}; for(auto &x:args) a<<x; a<<pkg+"/"+runner; return m.shell(serial,a,out); }
bool AndroidTestRunner::runShellTest(const QString &serial,const QString &cmd,QString *out) const { AndroidDeviceManager m; return m.shell(serial,{"sh","-c",cmd},out); }
