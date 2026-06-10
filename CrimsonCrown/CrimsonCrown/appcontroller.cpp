#include "appcontroller.h"
#include <QApplication>
#include <QFontDatabase>

///
/// \brief AppController::AppController
/// \details Constructor for the global application orchestrator.
/// Initializes the main window, loads the "Alagard" medieval pixel font,
/// connects all the signals from the menu UI, and starts the application.
/// \param parent Pointer to the parent QObject (defaults to nullptr).
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
    settingsController(new SettingController(mainWindow, this))
{
    mainWindow->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Police
    int fontId = QFontDatabase::addApplicationFont(":/alagard.ttf");
    if (fontId != -1) {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont customFont(family);
        qApp->setFont(customFont);
    }

    // Connects
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

    start();
}

///
/// \brief AppController::start
/// \details Launches the application by displaying the main menu.
///
void AppController::start() {
    navigateTo(AppState::Menu);
    mainWindow->show();
}

///
/// \brief AppController::onMenuRequested
/// \details Slot triggered when a request to return to the main menu is made.
///
void AppController::onMenuRequested() {
    navigateTo(AppState::Menu);
}

///
/// \brief AppController::onCharSelectRequested
/// \details Slot triggered when the 'PLAY' button is clicked.
///
void AppController::onCharSelectRequested() {
    navigateTo(AppState::CharSelect);
}

///
/// \brief AppController::onLeaderboardRequested
/// \details Slot triggered when the 'LEADERBOARD' button is clicked.
///
void AppController::onLeaderboardRequested() {
    // Next step: scoresController->createView();
}

///
/// \brief AppController::onSettingsRequested
/// \details Slot triggered when the 'SETTINGS' button is clicked.
///
void AppController::onSettingsRequested() {
    navigateTo(AppState::Setting);
}

///
/// \brief AppController::onGameRequested
/// \details Slot triggered to launch the main gameplay loop.
///
void AppController::onGameRequested(HeroType type) {
    selectedHero = type;
    navigateTo(AppState::Game);
}

///
/// \brief AppController::onQuitRequested
/// \details Slot triggered when the 'QUIT' button is clicked.
///
void AppController::onQuitRequested() {
    qApp->quit();
}

///
/// \brief AppController::navigateTo
/// \details Manages the application state machine and screen transitions.
/// Cleans up the active view, instantiates the requested view, and injects
/// it into the center of the QMainWindow.
/// \param state The new application state to navigate to.
///
void AppController::navigateTo(AppState state) {
    if (currentState == state && currentView != nullptr) return;

    cleanupCurrentView();

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
    default:
        return;
    }

    mainWindow->setCentralWidget(currentView);
    currentState = state;
}

///
/// \brief AppController::cleanupCurrentView
/// \details Safely frees memory and destroys the currently displayed view.
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
    }

    currentView = nullptr;
}

void AppController::onGameOver(int score, HeroType hero)
{
    gameController->destroyView();
    mainWindow->setCentralWidget(itsGameOverController->createView(score, hero));
}

void AppController::onGameWon(int score, HeroType hero)
{
    gameController->destroyView();
    mainWindow->setCentralWidget(itsGameWonController->createView(score, hero));
}