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

    // --- 🤫 MASQUAGE DES ZONES INVISIBLES (NIVEAU 1 & NIVEAU 3) ---

    // 1. Couloir secret du Niveau 1
    if (roomModel->getOffsetX() == 2100 && roomModel->getOffsetY() == 225) {
        return; // On stoppe le dessin : la pièce reste noire
    }

    // 2. Couloir et salle secrète du Niveau 3
    if ((roomModel->getOffsetX() == 3475 && roomModel->getOffsetY() == 1625) ||
        (roomModel->getOffsetX() == 3250 && roomModel->getOffsetY() == 2025))
    {
        return; // On stoppe le dessin : la pièce reste noire
    }

    // --- FIN DU MASQUAGE ---

    RoomType type = roomModel->getType();

    if (type == Spawn){
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

    painter.setPen(QPen(Qt::black, 4));
    for (const ObstacleModel* obstacle : roomModel->getItsObstacles()) {
        obstacleView.draw(painter, obstacle);
    }
}