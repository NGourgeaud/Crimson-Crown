#include "mainwindow.h"
#include "config.h"

///
/// \brief MainWindow::MainWindow
/// \details Constructor for the application's main window.
/// Configures the baseline window constraints (Title and dimensions).
/// \param parent Pointer to the parent QWidget
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(WINDOW_TITLE);
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

MainWindow::~MainWindow() {}
