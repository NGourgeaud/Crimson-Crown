#include <QApplication>
#include "appcontroller.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    AppController controller;

    return app.exec();
}
