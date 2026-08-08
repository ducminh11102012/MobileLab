#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("MobileLab Android");
    app.setOrganizationName("MobileLab");
    app.setApplicationDisplayName("MobileLab Android");

    MainWindow window;
    window.show();
    return app.exec();
}
