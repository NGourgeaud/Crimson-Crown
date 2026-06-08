#include "roomview.h"
#include <QBrush>
#include <QColor>
#include <QPen>

RoomView::RoomView()
{
    spawnTexture = QPixmap(":/sol-spawn.png");

}

void RoomView::draw(QPainter &painter, const RoomModel *roomModel, ObstacleView &obstacleView)
{
    if (!roomModel) return;

    RoomType type = roomModel->getType();

    if  (type == Spawn){
        painter.drawPixmap(0, 0, roomModel->getWidth(), roomModel->getHeight(), spawnTexture);
    }

    if (type == Boss) {
        painter.setBrush(QBrush(QColor(80, 20, 20)));
    }
    else {
        painter.setBrush(QBrush(QColor(64, 64, 64)));
    }

    painter.setPen(QPen(Qt::black, 4));
    painter.drawRect(0, 0, roomModel->getWidth(), roomModel->getHeight());


    // EFFET VISUEL : PALISSADES EN BOIS

    if (roomModel->getIsLocked()) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(QColor(139, 69, 19), 12));
        painter.drawRect(6, 6, roomModel->getWidth() - 12, roomModel->getHeight() - 12);
    }

    painter.setPen(QPen(Qt::black, 4));
    for (const ObstacleModel* obstacle : roomModel->getItsObstacles()) {
        obstacleView.draw(painter, obstacle);
    }
}