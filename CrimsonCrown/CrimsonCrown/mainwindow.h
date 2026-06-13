// File Name: mainwindow.h
// Group: A11
// Description: Header file defining the core main window host for the application.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ///
    /// \brief Constructor for the application's main window.
    /// \param parent Pointer to the parent QWidget.
    ///
    MainWindow(QWidget *parent = nullptr);

    ///
    /// \brief Destructor for the main window.
    ///
    ~MainWindow();
};

#endif // MAINWINDOW_H
