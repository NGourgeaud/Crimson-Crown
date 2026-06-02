#include "gamecontroller.h"

GameController::GameController() {
    // Créer le modèle et la vue
    itsModel = new GameModel();
    itsView  = new GameView(itsModel);

    // Relayer les événements clavier de la vue vers ce contrôleur
    connect(itsView, &GameView::keyPressed,  this, &GameController::onKeyPressed);
    connect(itsView, &GameView::keyReleased, this, &GameController::onKeyReleased);
}

void GameController::onKeyPressed(int key)
{
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

void GameController::onKeyReleased(int key)
{
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
