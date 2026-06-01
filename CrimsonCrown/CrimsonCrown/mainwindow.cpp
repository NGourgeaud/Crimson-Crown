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
    setWindowTitle(AppConfig::WINDOW_TITLE);
    resize(AppConfig::WINDOW_WIDTH, AppConfig::WINDOW_HEIGHT);
}

MainWindow::~MainWindow() {}
