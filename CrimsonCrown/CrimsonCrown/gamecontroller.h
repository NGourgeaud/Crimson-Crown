// File Name: gamecontroller.h
// Group: A11
// Description: Header file for the main game controller, managing the game loop, user inputs, and pause states.

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <cmath>
#include "gamemodel.h"
#include "gameview.h"

class GameController : public QObject
{
    Q_OBJECT
public:
    ///
    /// \brief Initializes the game controller.
    /// \param parentWidget The parent widget to host the game view.
    /// \param parent The parent QObject.
    ///
    explicit GameController(QWidget *parentWidget = nullptr, QObject *parent = nullptr);

    ///
    /// \brief Destructor handling the cleanup of the game model.
    ///
    ~GameController();

    ///
    /// \brief Instantiates the game viewport and starts the game loop.
    /// \param hero The specific hero type selected by the player.
    /// \return A pointer to the newly created GameView widget.
    ///
    QWidget *createView(HeroType hero);

    ///
    /// \brief Suspends loops and destroys the active view.
    ///
    void destroyView();

signals:
    /// \brief Emitted to request a return to the main menu.
    void menuRequested();

    /// \brief Emitted when the player dies.
    void gameOver(int score, HeroType hero);

    /// \brief Emitted when the final boss is defeated.
    void gameWon(int score, HeroType hero);

    /// \brief Emitted when a boss encounter begins.
    void bossEncountered();

    /// \brief Emitted when a boss is successfully killed.
    void bossKilled();

private slots:
    /// \brief Triggered upon player death.
    void onPlayerDeath();

    /// \brief Triggered upon boss death.
    void onBossDeath();

    /// \brief Main clock tick updating the game at 60 FPS.
    void onTick();

    /// \brief Handles raw keyboard press events.
    void onKeyPressed(int key);

    /// \brief Handles raw keyboard release events.
    void onKeyReleased(int key);

    /// \brief Toggles the game pause state.
    void togglePause();

    /// \brief Handles the transition when a level is completed.
    void onLevelCompleted(int nextLevelId);

    /// \brief Processes a purchase request from the shop.
    void onBuyUpgradeRequested(QString type);

    /// \brief Resumes the game and loads the next level.
    void onNextLevelRequested();

private:
    GameModel *itsModel;
    GameView *itsView;
    QWidget *parentWidget;
    QTimer *itsTimer;
    bool isPaused = false;
    int levelToLoadPending;
};

#endif // GAMECONTROLLER_H
