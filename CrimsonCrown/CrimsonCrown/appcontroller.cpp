#include "appcontroller.h"
#include <QApplication>
#include <QFontDatabase>
#include <QInputDialog>


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
    settingsController(new SettingController(mainWindow, this)),
    leaderboardController(new LeaderboardController(mainWindow, this))
{

    // Police
    int fontId = QFontDatabase::addApplicationFont(":/alagard.ttf");
    if (fontId != -1) {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont customFont(family);
        qApp->setFont(customFont);
    }
    QIcon appIcon(":/logo.png");
    qApp->setWindowIcon(appIcon);
    mainWindow->setWindowIcon(appIcon);

    // --- INITIALISATION DE L'AUDIO ---
    audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(0.5); // Volume à 50%

    musicPlayer = new QMediaPlayer(this);
    musicPlayer->setAudioOutput(audioOutput);
    musicPlayer->setLoops(QMediaPlayer::Infinite); // Boucle infinie pour que la musique ne s'arrête pas

    // Connexion pour la musique du Boss
    connect(gameController, &GameController::bossEncountered, this, &AppController::onBossEncountered);


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

    connect(gameController, &GameController::bossEncountered, this, &AppController::onBossEncountered);
    connect(gameController, &GameController::bossKilled, this, &AppController::onBossKilled); 

    connect(leaderboardController, &LeaderboardController::menuRequested, this, &AppController::onMenuRequested);
    connect(menuController, &MenuController::leaderboardRequested, this, &AppController::onLeaderboardRequested);

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
    navigateTo(AppState::Leaderboard);
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

    // --- CORRECTION : CHANGEMENT DE MUSIQUE SELON L'ÉTAT ---
    if (state == AppState::Menu) {
        // Se lance à chaque fois qu'on est sur un menu (au démarrage ou au retour)
        musicPlayer->setSource(QUrl("qrc:/menu-music.mp3"));
        musicPlayer->play();
    }
    else if (state == AppState::Game) {
        musicPlayer->setSource(QUrl("qrc:/combat-music.mp3"));
        musicPlayer->play();
    }

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
    } else if (currentState == AppState::Leaderboard) {
        leaderboardController->destroyView();
    }

    currentView = nullptr;
}

void AppController::onGameOver(int score, HeroType hero)
{

    if (currentState != AppState::Game) return;
    currentState = AppState::Menu; // Blocks other loop ticks

    gameController->destroyView();

    currentView = itsGameOverController->createView(score, hero);
    mainWindow->setCentralWidget(currentView);

    currentView = nullptr;

    QCoreApplication::processEvents();

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

    // Saved score in the Leaderboard
    leaderboardController->submitScore(playerName, score);
}

void AppController::onGameWon(int score, HeroType hero)
{
    // 1. Sécurité anti-double déclenchement
    if (currentState != AppState::Game) return;
    currentState = AppState::Menu;

    // 2. Nettoyage de l'écran de jeu
    gameController->destroyView();

    // 3. Affichage de l'écran de victoire
    currentView = itsGameWonController->createView(score, hero);
    mainWindow->setCentralWidget(currentView);

    // 4. On détache la vue et on force l'affichage
    currentView = nullptr;
    QCoreApplication::processEvents();

    // 5. Boîte de dialogue pour le prénom
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

    // 6. Sauvegarde du score
    leaderboardController->submitScore(playerName, score);
}

void AppController::onBossEncountered()
{
    musicPlayer->setSource(QUrl("qrc:/boss-music.mp3"));
    musicPlayer->play();
}

void AppController::onBossKilled()
{
    // Remettre la musique calme du donjon pendant que le joueur marche vers la sortie
    musicPlayer->setSource(QUrl("qrc:/combat-music.mp3"));
    musicPlayer->play();
}
