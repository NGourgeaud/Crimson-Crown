#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include "gameovercontroller.h"
#include "gamewoncontroller.h"
#include "mainwindow.h"
#include "menucontroller.h"
#include "charselectcontroller.h"
#include "gamecontroller.h"
#include "settingcontroller.h"

enum class AppState { Menu, CharSelect, Game, Setting };

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
    void onGameRequested(HeroType type);
    void onQuitRequested();
    void onGameOver(int score);
    void onGameWon(int score);

private:
    void navigateTo(AppState state);
    void cleanupCurrentView();

    MainWindow * mainWindow;
    MenuController * menuController;
    QWidget * currentView;
    AppState currentState;
    CharSelectController * charSelectController;
    GameController * gameController;
    HeroType selectedHero = HeroType::None;
    GameOverController * itsGameOverController;
    GameWonController * itsGameWonController;
    SettingController * settingsController;
};

#endif // APPCONTROLLER_H
