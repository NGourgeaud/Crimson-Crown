#include "gameview.h"

GameView::GameView(GameModel * aModel) {
    itsModel = aModel;
}

void GameView::keyPressEvent(QKeyEvent * event)
{
    if (!event->isAutoRepeat())
        emit keyPressed(event->key());
}

void GameView::keyReleaseEvent(QKeyEvent * event)
{
    if (!event->isAutoRepeat())
        emit keyReleased(event->key());
}
