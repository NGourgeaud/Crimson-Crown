#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include "mainwindow.h"
#include "menucontroller.h"

enum class AppState { Menu, CharSelect, Game };

class AppController : public QObject {
    Q_OBJECT
public:
    explicit AppController(QObject *parent = nullptr);
    void start();

private slots:
    void onMenuRequested();
    void onCharSelectRequested();
    void onLeaderboardRequested();
    void onSettingsRequested();
    void onGameRequested();
    void onQuitRequested();
    void onGameOver(int score);

private:
    void navigateTo(AppState state);
    void cleanupCurrentView();

    MainWindow *mainWindow;
    MenuController *menuController;
    QWidget *currentView;
    AppState currentState;
};

#endif // APPCONTROLLER_H
