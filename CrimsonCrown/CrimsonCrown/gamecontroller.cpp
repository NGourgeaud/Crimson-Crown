// File Name: gamecontroller.cpp
// Group: A11
// Description: Core controller managing the main game loop, physical updates, input routing, and game states (pause, shop, game over).

#include "gamecontroller.h"

///
/// \brief Initializes the game controller, its model, and the high-precision frame timer.
/// \param parentWidget The parent widget to host the game view.
/// \param parent The parent QObject for memory management.
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
/// \brief Destructor handling the cleanup of the underlying game model.
///
GameController::~GameController()
{
    delete itsModel;
}

///
/// \brief Instantiates the hardware-focused viewport, routes device listeners, and spins up physics processing.
/// \param hero The specific hero type selected by the player.
/// \return A pointer to the newly created GameView widget.
///
QWidget* GameController::createView(HeroType hero)
{
    isPaused = false;
    levelToLoadPending = 1; // Ensures the level counter resets correctly on a new game

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
/// \brief Suspends execution loops and clears graphical handles from volatile layers.
///
void GameController::destroyView()
{
    if (itsView == nullptr) return;

    itsTimer->stop();
    disconnect(itsView);
    itsView->deleteLater();
    itsView = nullptr;
}

///
/// \brief Slot triggered when the player's health reaches zero, emitting the game over state.
///
void GameController::onPlayerDeath()
{
    emit gameOver(itsModel->getItsScore(), itsModel->getItsPlayer()->getItsName());
}

///
/// \brief Slot triggered when the final boss is defeated, emitting the victory state.
///
void GameController::onBossDeath()
{
    emit gameWon(itsModel->getItsScore(), itsModel->getItsPlayer()->getItsName());
}

///
/// \brief Core heartbeat of the game engine, updating physics and rendering at 60 FPS.
///
void GameController::onTick()
{
    if (!isPaused) {
        itsModel->updateLogic(); // The game logic stops calculating if isPaused == true!
    }

    if (itsView != nullptr) {
        itsView->update(); // We continue rendering the view to display the pause/shop screens
    }
}

///
/// \brief Routes raw keyboard press events from the view to the model's action states.
/// \param key The integer code of the pressed key.
///
void GameController::onKeyPressed(int key)
{
    if (key == Qt::Key_Escape || key == Qt::Key_P) {
        togglePause();
        return;
    }

    if (isPaused) return;

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
    else if (key == keyBind::keyAttaque1) {
        itsModel->getItsPlayer()->setAttack1(true);
    }
    else if (key == keyBind::keyAttaque2) {
        itsModel->getItsPlayer()->setAttack2(true);
    }
}

///
/// \brief Routes raw keyboard release events to stop player movements or attacks.
/// \param key The integer code of the released key.
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

///
/// \brief Toggles the game's paused state and toggles the pause menu visibility.
///
void GameController::togglePause()
{
    isPaused = !isPaused;
    if (itsView != nullptr) {
        itsView->setPauseMenuVisible(isPaused);
    }
}

///
/// \brief Handles the transition sequence when a level is completed, opening the shop menu.
/// \param nextLevelId The ID of the upcoming level to be loaded.
///
void GameController::onLevelCompleted(int nextLevelId) {
    levelToLoadPending = nextLevelId;
    isPaused = true; // Stops the game logic while in the shop
    itsView->setShopMenuVisible(true);
}

///
/// \brief Processes a purchase request from the shop menu, verifying funds and applying the upgrade.
/// \param type A string identifier for the requested upgrade ("HP", "DMG", or "SPEED").
///
void GameController::onBuyUpgradeRequested(QString type) {
    bool success = false;

    // Calculates the price multiplier based on the current level of the model
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

///
/// \brief Closes the shop, loads the pending level, and resumes the game loop.
///
void GameController::onNextLevelRequested() {
    itsView->setShopMenuVisible(false);
    itsModel->loadLevel(levelToLoadPending);
    isPaused = false; // Resumes the game logic
}
