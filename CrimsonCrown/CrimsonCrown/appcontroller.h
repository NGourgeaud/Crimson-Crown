// File Name: appcontroller.h
// Group: A11
// Description: Header file for the main application orchestrator, defining state transitions and UI connections.

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
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QIcon>
#include "leaderboardcontroller.h"

enum class AppState { Menu, CharSelect, Game, Setting, Leaderboard };

class AppController : public QObject {
    Q_OBJECT
public:
    ///
    /// \brief Constructor for the global application orchestrator.
    /// \param parent Pointer to the parent QObject.
    ///
    explicit AppController(QObject *parent = nullptr);

    ///
    /// \brief Launches the application by displaying the main window.
    ///
    void start();

    ///
    /// \brief Manages the application state machine and updates the central view widget.
    /// \param state The new application state to navigate to.
    ///
    void navigateTo(AppState state);

    ///
    /// \brief Safely frees memory and destroys the currently displayed view.
    ///
    void cleanupCurrentView();

private slots:
    ///
    /// \brief Triggers a state change back to the main menu.
    ///
    void onMenuRequested();

    ///
    /// \brief Triggers a state change to the character selection screen.
    ///
    void onCharSelectRequested();

    ///
    /// \brief Triggers a state change to the leaderboard screen.
    ///
    void onLeaderboardRequested();

    ///
    /// \brief Triggers a state change to the settings screen.
    ///
    void onSettingsRequested();

    ///
    /// \brief Triggers a state change to start the main game loop with a specific hero.
    /// \param type The specific type of hero selected by the player.
    ///
    void onGameRequested(HeroType type);

    ///
    /// \brief Triggers the application shutdown sequence.
    ///
    void onQuitRequested();

    ///
    /// \brief Handles the game over sequence.
    /// \param score The final score achieved by the player.
    /// \param hero The type of hero the player was using when they died.
    ///
    void onGameOver(int score, HeroType hero);

    ///
    /// \brief Handles the victory sequence.
    /// \param score The final score achieved by the player upon winning.
    /// \param hero The type of hero the player used to win.
    ///
    void onGameWon(int score, HeroType hero);

public slots:
    ///
    /// \brief Switches the background music to the boss theme.
    ///
    void onBossEncountered();

    ///
    /// \brief Reverts the background music to the standard theme.
    ///
    void onBossKilled();

private:

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
    LeaderboardController * leaderboardController;
    QMediaPlayer * musicPlayer;
    QAudioOutput * audioOutput;
};

#endif // APPCONTROLLER_H
