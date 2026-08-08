#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QListWidget>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTextEdit>
#include <QProgressBar>
#include <QTimer>

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QWidget *makeSidebar();
    QWidget *makeTopBar();
    QWidget *makeDashboard();
    QWidget *makeDevices();
    QWidget *makeMatrix();
    QWidget *makeConsole();
    QWidget *makeInspector();
    QPushButton *navButton(const QString &text, const QString &key);
    QLabel *statusLabel(const QString &icon, const QString &text);
    void selectPage(int index);

    QStackedWidget *pages_ = nullptr;
    QListWidget *deviceList_ = nullptr;
    QTableWidget *matrix_ = nullptr;
    QTextEdit *console_ = nullptr;
    QLabel *connection_ = nullptr;
    QLabel *active_ = nullptr;
    QLabel *errors_ = nullptr;
    QProgressBar *cpu_ = nullptr;
    QProgressBar *memory_ = nullptr;
    QTimer *pollTimer_ = nullptr;
};
