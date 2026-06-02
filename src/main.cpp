#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Pirata Game");
    app.setApplicationVersion("0.1");

    MainWindow window;
    window.setWindowTitle("El Tormenta Escarlata");
    window.resize(1280, 720);
    window.show();

    return app.exec();
}
