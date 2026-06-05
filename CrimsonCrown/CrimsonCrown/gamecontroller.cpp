#include "gamecontroller.h"

///
/// \brief GameController::GameController
///
GameController::GameController(QWidget *parentWidget, QObject *parent)
    : QObject(parent),
    itsModel(new GameModel()),
    itsView(nullptr),
    parentWidget(parentWidget)
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
    itsModel->resetGame(hero);

    if (itsModel->getItsPlayer()) {
        connect(itsModel->getItsPlayer(), &PlayerModel::death, this, &GameController::onPlayerDeath);
    }
    connect(itsModel, &GameModel::gameWon, this, &GameController::gameWon);

    itsView = new GameView(itsModel, parentWidget);

    connect(itsView, &GameView::keyPressed, this, &GameController::onKeyPressed);
    connect(itsView, &GameView::keyReleased, this, &GameController::onKeyReleased);
    connect(itsModel, &GameModel::attack, itsView, &GameView::drawAttackArea);

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
    emit gameOver(itsModel->getItsScore());
}

void GameController::onBossDeath()
{
    emit gameWon(itsModel->getItsScore());
}



///
/// \brief GameController::onTick
///
void GameController::onTick()
{
    // Advance simulation frames (Uncomment when update loops are deployed in GameModel)
    itsModel->updateLogic();

    if (itsView != nullptr) {
        itsView->update();
    }
}

///
/// \brief GameController::onKeyPressed
///
void GameController::onKeyPressed(int key)
{
    // Safe guard model verification passes
    if (!itsModel || !itsModel->getItsPlayer()) return;

    switch (key) {
    case keyBind::keyLeft:
        itsModel->getItsPlayer()->setMovementLeft(true);
        break;
    case keyBind::keyRight:
        itsModel->getItsPlayer()->setMovementRight(true);
        break;
    case keyBind::keyUp:
        itsModel->getItsPlayer()->setMovementUp(true);
        break;
    case keyBind::keyDown:
        itsModel->getItsPlayer()->setMovementDown(true);
        break;
    // Touche de kill ennemis
    case Qt::Key_T:
        itsModel->cheatKillEnemies();
        break;
    case keyBind::keyAttaque1:
        itsModel->getItsPlayer()->setAttack1(true);
    case keyBind::keyAttaque2:
        break;
        itsModel->getItsPlayer()->setAttack2(true);
    default:
        break;
    }
}

///
/// \brief GameController::onKeyReleased
///
void GameController::onKeyReleased(int key)
{
    if (!itsModel || !itsModel->getItsPlayer()) return;

    switch (key) {
    case keyBind::keyLeft:
        itsModel->getItsPlayer()->setMovementLeft(false);
        break;
    case keyBind::keyRight:
        itsModel->getItsPlayer()->setMovementRight(false);
        break;
    case keyBind::keyUp:
        itsModel->getItsPlayer()->setMovementUp(false);
        break;
    case keyBind::keyDown:
        itsModel->getItsPlayer()->setMovementDown(false);
        break;
    case keyBind::keyAttaque1:
        itsModel->getItsPlayer()->setAttack1(false);
        break;
    case keyBind::keyAttaque2:
        itsModel->getItsPlayer()->setAttack2(false);
    default:
        break;
    }
}
