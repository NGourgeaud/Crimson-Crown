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

    // --- 🤫 MASQUAGE DES ZONES INVISIBLES ---
    if (roomModel->getOffsetX() == 2100 && roomModel->getOffsetY() == 225) return;
    if ((roomModel->getOffsetX() == 3475 && roomModel->getOffsetY() == 1625) ||
        (roomModel->getOffsetX() == 3250 && roomModel->getOffsetY() == 2025)) return;

    RoomType type = roomModel->getType();

    // --- DESSIN DU FOND DE LA SALLE ---
    if (type == RoomType::Spawn) {
        if (!spawnTexture.isNull()) {
            painter.drawPixmap(0, 0, roomModel->getWidth(), roomModel->getHeight(), spawnTexture);
            painter.setBrush(Qt::NoBrush); // Empêche le gris de recouvrir la texture
        } else {
            painter.setBrush(QBrush(QColor(64, 64, 64))); // SÉCURITÉ : Gris clair au lieu de noir
        }
    }
    else if (type == RoomType::Boss) {
        painter.setBrush(QBrush(QColor(80, 20, 20))); // Rouge sombre
    }
    else {
        painter.setBrush(QBrush(QColor(64, 64, 64))); // Gris classique
    }

    // CONTOUR DE LA PIÈCE
    painter.setPen(QPen(Qt::black, 4));
    painter.drawRect(0, 0, roomModel->getWidth(), roomModel->getHeight());

    // DESSIN DES OBSTACLES
    painter.setPen(QPen(Qt::black, 4));
    for (const ObstacleModel* obstacle : roomModel->getItsObstacles()) {
        obstacleView.draw(painter, obstacle);
    }
}