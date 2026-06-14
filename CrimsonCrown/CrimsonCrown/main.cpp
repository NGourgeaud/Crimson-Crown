// File Name: main.cpp
// Group: A11
// Description: Entry point of the application, initializing the QApplication and launching the main controller.

#include <QApplication>
#include "appcontroller.h"

///
/// \brief Main entry point of the application.
/// \param argc Argument count.
/// \param argv Argument vector.
/// \return Application exit code.
///
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    AppController controller;

    return app.exec();
}
