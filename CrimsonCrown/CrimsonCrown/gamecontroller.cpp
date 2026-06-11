#include "gamecontroller.h"

///
/// \brief GameController::GameController
///
GameController::GameController(QWidget *parentWidget, QObject *parent)
    : QObject(parent),
    itsModel(new GameModel()),
    itsView(nullptr),
    parentWidget(parentWidget),
    levelToLoadPending(1)
{
    // High-precision clock driving game frames at roughly 60 FPS (16ms)
    itsTimer = new QTimer(this);
    connect(itsTimer, &QTimer::timeout, this, &GameController::onTick);
}

///
/// \brief GameController::~GameController
///
GameController::~GameController()
{
    delete itsModel;
}

///
/// \brief GameController::createView
/// \details Instantiates the hardware-focused viewport, routes device listeners, and spins up physics processing.
///
QWidget* GameController::createView(HeroType hero)
{
    isPaused = false;

    itsModel->resetGame(hero);

    if (itsModel->getItsPlayer()) {
        connect(itsModel->getItsPlayer(), &PlayerModel::death, this, &GameController::onPlayerDeath);
    }
    connect(itsModel, &GameModel::gameWon, this, &GameController::gameWon, Qt::UniqueConnection);

    itsView = new GameView(itsModel, parentWidget);

    connect(itsView, &GameView::keyPressed, this, &GameController::onKeyPressed);
    connect(itsView, &GameView::keyReleased, this, &GameController::onKeyReleased);
    connect(itsModel, &GameModel::attack, itsView, &GameView::drawAttackArea, Qt::UniqueConnection);

    connect(itsView, &GameView::quitRequested, this, &GameController::menuRequested);
    connect(itsModel, &GameModel::bossSpawned, this, &GameController::bossEncountered);
    connect(itsModel, &GameModel::bossKilled, this, &GameController::bossKilled);

    connect(itsModel, &GameModel::levelCompleted, this, &GameController::onLevelCompleted);
    connect(itsView, &GameView::buyUpgradeRequested, this, &GameController::onBuyUpgradeRequested);
    connect(itsView, &GameView::nextLevelRequested, this, &GameController::onNextLevelRequested);

    itsView->setFocus();

    itsTimer->start(16);
    return itsView;
}

///
/// \brief GameController::destroyView
/// \details Suspends execution loops and clears graphical handles from volatile layers.
///
void GameController::destroyView()
{
    if (itsView == nullptr) return;

    itsTimer->stop();
    disconnect(itsView);
    itsView->deleteLater();
    itsView = nullptr;
}

void GameController::onPlayerDeath()
{
    emit gameOver(itsModel->getItsScore(), itsModel->getItsPlayer()->getItsName());
}

void GameController::onBossDeath()
{
    emit gameWon(itsModel->getItsScore(), itsModel->getItsPlayer()->getItsName());
}



///
/// \brief GameController::onTick
///
void GameController::onTick()
{
    if (!isPaused) {
        itsModel->updateLogic(); // The game stops calculating if isPaused == true!
    }

    if (itsView != nullptr) {
        itsView->update(); // We continue drawing to see the pause screen
    }
}

///
/// \brief GameController::onKeyPressed
///
void GameController::onKeyPressed(int key)
{
    if (key == Qt::Key_Escape || key == Qt::Key_P) {
        togglePause();
        return;
    }
    if (isPaused) return;


    if (!itsModel || !itsModel->getItsPlayer()) return;
    // Safe guard model verification passes
    if (!itsModel || !itsModel->getItsPlayer()) return;

    if (key == keyBind::keyLeft) {
        itsModel->getItsPlayer()->setMovementLeft(true);
    }
    else if (key == keyBind::keyRight) {
        itsModel->getItsPlayer()->setMovementRight(true);
    }
    else if (key == keyBind::keyUp) {
        itsModel->getItsPlayer()->setMovementUp(true);
    }
    else if (key == keyBind::keyDown) {
        itsModel->getItsPlayer()->setMovementDown(true);
    }
    else if (key == Qt::Key_T) {
        itsModel->cheatKillEnemies();
    }
    else if (key == keyBind::keyAttaque1) {
        itsModel->getItsPlayer()->setAttack1(true);
    }
    else if (key == keyBind::keyAttaque2) {
        itsModel->getItsPlayer()->setAttack2(true);
    }
}

///
/// \brief GameController::onKeyReleased
///
void GameController::onKeyReleased(int key)
{
    if (!itsModel || !itsModel->getItsPlayer()) return;

    if (key == keyBind::keyLeft) {
        itsModel->getItsPlayer()->setMovementLeft(false);
    }
    else if (key == keyBind::keyRight) {
        itsModel->getItsPlayer()->setMovementRight(false);
    }
    else if (key == keyBind::keyUp) {
        itsModel->getItsPlayer()->setMovementUp(false);
    }
    else if (key == keyBind::keyDown) {
        itsModel->getItsPlayer()->setMovementDown(false);
    }
    else if (key == keyBind::keyAttaque1) {
        itsModel->getItsPlayer()->setAttack1(false);
    }
    else if (key == keyBind::keyAttaque2) {
        itsModel->getItsPlayer()->setAttack2(false);
    }
}

void GameController::togglePause()
{
    isPaused = !isPaused;
    if (itsView != nullptr) {
        itsView->setPauseMenuVisible(isPaused);
    }
}

void GameController::onLevelCompleted(int nextLevelId) {
    levelToLoadPending = nextLevelId;
    isPaused = true; // On stop la logique du jeu
    itsView->setShopMenuVisible(true);
}

void GameController::onBuyUpgradeRequested(QString type) {
    bool success = false;

    // Calcul du multiplicateur de prix selon le niveau actuel du modèle
    int level = itsModel->getCurrentLevel();
    double multiplier = std::pow(PRICE_SCALE, level - 2);

    if (type == "HP") {
        int price = static_cast<int>(BASE_PRICE_HP * multiplier);
        if (itsModel->spendGold(price)) {
            itsModel->getItsPlayer()->increaseMaxHP(5);
            success = true;
        }
    }
    else if (type == "DMG") {
        int price = static_cast<int>(BASE_PRICE_DMG * multiplier);
        if (itsModel->spendGold(price)) {
            itsModel->getItsPlayer()->increaseDamage(0.20);
            success = true;
        }
    }
    else if (type == "SPEED") {
        int price = static_cast<int>(BASE_PRICE_SPEED * multiplier);
        if (itsModel->spendGold(price)) {
            itsModel->getItsPlayer()->increaseSpeed(2);
            success = true;
        }
    }

    if (success) {
        itsView->updateShopGold(itsModel->getGoldAmount());
        itsView->update();
    }
}

void GameController::onNextLevelRequested() {
    itsView->setShopMenuVisible(false);
    itsModel->loadLevel(levelToLoadPending);
    isPaused = false; // On relance le jeu
}
