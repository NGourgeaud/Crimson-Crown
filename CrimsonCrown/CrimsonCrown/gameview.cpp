#include "gameview.h"

///
/// \brief GameView::GameView
///
GameView::GameView(GameModel *model, QWidget *parent)
    : QWidget(parent), itsModel(model)
{
}

///
/// \brief GameView::keyPressEvent
///
void GameView::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        emit keyPressed(event->key());
    }
}

///
/// \brief GameView::keyReleaseEvent
///
void GameView::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        emit keyReleased(event->key());
    }
}

///
/// \brief GameView::paintEvent
/// \details Rendering pass processing spatial structures, map tile sheets and dynamic layers.
///
void GameView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // example
    painter.fillRect(this->rect(), QColor(20, 15, 20));
    painter.setPen(Qt::white);
    QFont gameFont("Alagard", 20);
    painter.setFont(gameFont);
    painter.drawText(this->rect(), Qt::AlignCenter, "--- GAME ---");
}
