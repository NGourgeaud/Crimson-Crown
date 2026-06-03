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
QWidget* GameController::createView()
{
    itsView = new GameView(itsModel, parentWidget);

    // Wire physical keyboard abstractions down to functional coordinate mutations
    connect(itsView, &GameView::keyPressed, this, &GameController::onKeyPressed);
    connect(itsView, &GameView::keyReleased, this, &GameController::onKeyReleased);

    // Boot game simulation ticking
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

///
/// \brief GameController::onTick
///
void GameController::onTick()
{
    // Advance simulation frames (Uncomment when update loops are deployed in GameModel)
    // itsModel->updateLogic();

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
    case Qt::Key_Left:
    case Qt::Key_Q:
        itsModel->getItsPlayer()->setMovementLeft(true);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        itsModel->getItsPlayer()->setMovementRight(true);
        break;
    case Qt::Key_Up:
    case Qt::Key_Z:
        itsModel->getItsPlayer()->setMovementUp(true);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        itsModel->getItsPlayer()->setMovementDown(true);
        break;
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
    case Qt::Key_Left:
    case Qt::Key_Q:
        itsModel->getItsPlayer()->setMovementLeft(false);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        itsModel->getItsPlayer()->setMovementRight(false);
        break;
    case Qt::Key_Up:
    case Qt::Key_Z:
        itsModel->getItsPlayer()->setMovementUp(false);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        itsModel->getItsPlayer()->setMovementDown(false);
        break;
    default:
        break;
    }
}
