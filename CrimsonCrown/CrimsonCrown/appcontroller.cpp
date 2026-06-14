// File Name: appcontroller.cpp
// Group: A11
// Description: Main controller orchestrating the MVC application state, UI transitions, and audio logic for the game.

#include "appcontroller.h"
#include <QApplication>
#include <QFontDatabase>
#include <QInputDialog>

///
/// \brief Constructor for the global application orchestrator, initializing audio, UI, and connections.
/// \param parent Pointer to the parent QObject.
///
AppController::AppController(QObject *parent)
    : QObject(parent),
    mainWindow(new MainWindow()),
    menuController(new MenuController(mainWindow, this)),
    currentView(nullptr),
    currentState(AppState::Menu),
    charSelectController(new CharSelectController(mainWindow, this)),
    gameController(new GameController(mainWindow, this)),
    itsGameOverController(new GameOverController()),
    itsGameWonController(new GameWonController()),
    settingsController(new SettingController(mainWindow, this)),
    leaderboardController(new LeaderboardController(mainWindow, this))
{

    // Font initialization
    int fontId = QFontDatabase::addApplicationFont(":/alagard.ttf");
    if (fontId != -1) {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont customFont(family);
        qApp->setFont(customFont);
    }
    QIcon appIcon(":/logo.png");
    qApp->setWindowIcon(appIcon);
    mainWindow->setWindowIcon(appIcon);

    // Audio initialization
    audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(0.5); // Volume set to 50%

    musicPlayer = new QMediaPlayer(this);
    musicPlayer->setAudioOutput(audioOutput);
    musicPlayer->setLoops(QMediaPlayer::Infinite); // Infinite loop so the music never stops

    // Signal and slot connections
    connect(menuController, &MenuController::playRequested, this, &AppController::onCharSelectRequested);
    connect(menuController, &MenuController::quitRequested, this, &AppController::onQuitRequested);

    connect(charSelectController, &CharSelectController::backToMenuRequested, this, &AppController::onMenuRequested);
    connect(charSelectController, &CharSelectController::characterSelected, this, &AppController::onGameRequested);

    connect(settingsController, &SettingController::menuRequested, this, &AppController::onMenuRequested);
    connect(menuController, &MenuController::settingsRequested, this, &AppController::onSettingsRequested);

    connect(gameController, &GameController::menuRequested, this, &AppController::onMenuRequested);
    connect(gameController, &GameController::gameOver, this, &AppController::onGameOver);
    connect(gameController, &GameController::gameWon, this, &AppController::onGameWon);

    connect(itsGameOverController, &GameOverController::menuRequested, this, &AppController::onMenuRequested);
    connect(itsGameWonController, &GameWonController::menuRequested, this, &AppController::onMenuRequested);

    connect(gameController, &GameController::bossEncountered, this, &AppController::onBossEncountered);
    connect(gameController, &GameController::bossKilled, this, &AppController::onBossKilled);

    connect(leaderboardController, &LeaderboardController::menuRequested, this, &AppController::onMenuRequested);
    connect(menuController, &MenuController::leaderboardRequested, this, &AppController::onLeaderboardRequested);

    connect(settingsController, &SettingController::globalVolumeChanged, this, [this](int volume) {
        // Converts 0-100 (from the slider) to 0.0-1.0 (for QAudioOutput)
        audioOutput->setVolume(volume / 100.0f);
    });

    start();
}

///
/// \brief Launches the application by displaying the main window and navigating to the menu.
///
void AppController::start() {
    navigateTo(AppState::Menu);
    mainWindow->show();
}

///
/// \brief Triggers a state change back to the main menu.
///
void AppController::onMenuRequested() {
    navigateTo(AppState::Menu);
}

///
/// \brief Triggers a state change to the character selection screen.
///
void AppController::onCharSelectRequested() {
    navigateTo(AppState::CharSelect);
}

///
/// \brief Triggers a state change to the leaderboard screen.
///
void AppController::onLeaderboardRequested() {
    navigateTo(AppState::Leaderboard);
}

///
/// \brief Triggers a state change to the settings screen.
///
void AppController::onSettingsRequested() {
    navigateTo(AppState::Setting);
}

///
/// \brief Triggers a state change to start the main game loop with a specific hero.
/// \param type The specific type of hero selected by the player.
///
void AppController::onGameRequested(HeroType type) {
    selectedHero = type;
    navigateTo(AppState::Game);
}

///
/// \brief Triggers the application shutdown sequence.
///
void AppController::onQuitRequested() {
    qApp->quit();
}

///
/// \brief Manages the application state machine and updates the central view widget accordingly.
/// \param state The new application state to navigate to.
///
void AppController::navigateTo(AppState state) {
    if (currentState == state && currentView != nullptr) return;

    cleanupCurrentView();

    // Manage background music transitions based on the target state
    if (state == AppState::Menu) {
        QUrl menuSource("qrc:/menu-music.mp3");
        // The file is only reloaded if it's not already the one being played
        if (musicPlayer->source() != menuSource) {
            musicPlayer->setSource(menuSource);
            musicPlayer->play();
        }
    }
    else if (state == AppState::Game) {
        QUrl combatSource("qrc:/combat-music.mp3");
        if (musicPlayer->source() != combatSource) {
            musicPlayer->setSource(combatSource);
            musicPlayer->play();
        }
    }

    // Instantiate the appropriate view based on the state
    switch (state) {
    case AppState::Menu:
        currentView = menuController->createView();
        break;
    case AppState::CharSelect:
        currentView = charSelectController->createView();
        break;
    case AppState::Game:
        currentView = gameController->createView(selectedHero);
        break;
    case AppState::Setting:
        currentView = settingsController->createView();
        break;
    case AppState::Leaderboard:
        currentView = leaderboardController->createView();
        break;
    default:
        return;
    }

    mainWindow->setCentralWidget(currentView);
    currentState = state;
}

///
/// \brief Safely frees memory and destroys the currently displayed view based on the active state.
///
void AppController::cleanupCurrentView() {
    if (currentView == nullptr) return;

    if (currentState == AppState::Menu) {
        menuController->destroyView();
    } else if (currentState == AppState::CharSelect) {
        charSelectController->destroyView();
    } else if (currentState == AppState::Game) {
        gameController->destroyView();
    } else if (currentState == AppState::Setting) {
        settingsController->destroyView();
    } else if (currentState == AppState::Leaderboard) {
        leaderboardController->destroyView();
    } else if (currentState == AppState::GameOver) {
        itsGameOverController->destroyView();
    } else if (currentState == AppState::GameWon) {
        itsGameWonController->destroyView();
    }

    currentView = nullptr;
}

///
/// \brief Handles the game over sequence, displays the final score screen, and prompts for a leaderboard name.
/// \param score The final score achieved by the player.
/// \param hero The type of hero the player was using when they died.
///
void AppController::onGameOver(int score, HeroType hero)
{
    if (currentState != AppState::Game) return;
    currentState = AppState::GameOver;

    gameController->destroyView();

    currentView = itsGameOverController->createView(score, hero);
    mainWindow->setCentralWidget(currentView);

    QCoreApplication::processEvents();

    // Prompt user for their name to save the score
    bool ok;
    QString playerName = QInputDialog::getText(mainWindow,
                                               "Game Over",
                                               "Entrez votre prénom pour le Leaderboard :",
                                               QLineEdit::Normal,
                                               "Joueur", &ok);

    if (!ok || playerName.trimmed().isEmpty()) {
        playerName = "Anonyme";
    } else {
        playerName = playerName.trimmed();
    }

    // Save score in the Leaderboard
    leaderboardController->submitScore(playerName, score);
}

///
/// \brief Handles the victory sequence, displays the win screen, and prompts for a leaderboard name.
/// \param score The final score achieved by the player upon winning.
/// \param hero The type of hero the player used to win.
///
void AppController::onGameWon(int score, HeroType hero)
{
    // 1. Anti-double trigger security
    if (currentState != AppState::Game) return;
    currentState = AppState::GameWon;

    // 2. Clean up the game screen
    gameController->destroyView();

    // 3. Display the victory screen
    currentView = itsGameWonController->createView(score, hero);
    mainWindow->setCentralWidget(currentView);

    // 4. Force display update
    QCoreApplication::processEvents();

    // 5. Dialog box for the player's name
    bool ok;
    QString playerName = QInputDialog::getText(mainWindow,
                                               "Victoire !",
                                               "Félicitations ! Entrez votre prénom :",
                                               QLineEdit::Normal,
                                               "Joueur", &ok);

    if (!ok || playerName.trimmed().isEmpty()) {
        playerName = "Anonyme";
    } else {
        playerName = playerName.trimmed();
    }

    // 6. Save the score
    leaderboardController->submitScore(playerName, score);
}

///
/// \brief Switches the background music to the boss theme when a boss battle starts.
///
void AppController::onBossEncountered()
{
    musicPlayer->setSource(QUrl("qrc:/boss-music.mp3"));
    musicPlayer->play();
}

///
/// \brief Reverts the background music to the standard dungeon theme after defeating a boss.
///
void AppController::onBossKilled()
{
    // Revert to calm dungeon music while the player walks to the exit
    musicPlayer->setSource(QUrl("qrc:/combat-music.mp3"));
    musicPlayer->play();
}
