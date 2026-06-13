// File Name: mainwindow.cpp
// Group: A11
// Description: Core window host for the application, enforcing baseline constraints like dimensions and titles.

#include "mainwindow.h"
#include "config.h"

///
/// \brief Constructor for the application's main window.
/// \param parent Pointer to the parent QWidget.
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(WINDOW_TITLE);
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

///
/// \brief Destructor for the main window.
///
MainWindow::~MainWindow() {}
