#include "roomview.h"
#include <QBrush>
#include <QColor>
#include <QPen>

RoomView::RoomView()
{
}

void RoomView::draw(QPainter &painter, const RoomModel *roomModel, ObstacleView &obstacleView)
{
    if (!roomModel) return;

    RoomType type = roomModel->getType();

    // Couleur du sol
    if (type == Boss) {
        painter.setBrush(QBrush(QColor(80, 20, 20))); // Boss en rouge
    }
    else {
        painter.setBrush(QBrush(QColor(64, 64, 64))); // Gris normal
    }

    painter.setPen(QPen(Qt::black, 4));

    // MAGIE : Le rectangle s'adapte automatiquement à la taille dictée par le modèle !
    painter.drawRect(0, 0, roomModel->getWidth(), roomModel->getHeight());

    // Dessin des obstacles
    for (const ObstacleModel* obstacle : roomModel->getItsObstacles()) {
        obstacleView.draw(painter, obstacle);
    }
}