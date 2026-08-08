#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QListWidget>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTextEdit>
#include <QProgressBar>
#include <QTimer>
class AndroidRuntime; class ResourceScheduler; class ApiServer;
class MainWindow final:public QMainWindow{
 Q_OBJECT
public:explicit MainWindow(QWidget*=nullptr);
private:QWidget*makeSidebar();QWidget*makeTopBar();QWidget*makeDashboard();QWidget*makeDevices();QWidget*makeMatrix();QWidget*makeConsole();QWidget*makeInspector();void selectPage(int);
 QStackedWidget*pages_=nullptr;QListWidget*deviceList_=nullptr;QTableWidget*matrix_=nullptr;QTextEdit*console_=nullptr;QLabel*connection_=nullptr;QLabel*active_=nullptr;QProgressBar*cpu_=nullptr;QProgressBar*memory_=nullptr;QTimer*pollTimer_=nullptr;AndroidRuntime*runtime_=nullptr;ResourceScheduler*scheduler_=nullptr;ApiServer*api_=nullptr;
};
