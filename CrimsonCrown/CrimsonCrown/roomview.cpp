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
        return;
    }

    // 2. Couloir et salle secrète du Niveau 3
    if ((roomModel->getOffsetX() == 3475 && roomModel->getOffsetY() == 1625) ||
        (roomModel->getOffsetX() == 3250 && roomModel->getOffsetY() == 2025))
    {
        return;
    }
    // --- FIN DU MASQUAGE ---

    RoomType type = roomModel->getType();

    // DESSIN DU FOND DE LA SALLE
    if (type == Spawn){
        painter.drawPixmap(0, 0, roomModel->getWidth(), roomModel->getHeight(), spawnTexture);
    }
    else if (type == Boss) {
        painter.setBrush(QBrush(QColor(80, 20, 20))); // Rouge sombre
    }
    else {
        painter.setBrush(QBrush(QColor(64, 64, 64))); // Gris classique
    }

    // CONTOUR DE LA PIÈCE
    painter.setPen(QPen(Qt::black, 4));
    painter.drawRect(0, 0, roomModel->getWidth(), roomModel->getHeight());

    // ==========================================
    // 🚪 DESSIN DES BARRICADES (PORTES VERROUILLÉES)
    // ==========================================
    if (roomModel->getIsLocked()) {
        painter.save();
        painter.setBrush(QColor(139, 69, 19)); // Couleur bois
        painter.setPen(QPen(Qt::black, 3));

        int w = roomModel->getWidth();
        int h = roomModel->getHeight();

        // Poutre Haut (Nord)
        painter.drawRect((w / 2) - 75, 0, 150, 20);

        // Poutre Bas (Sud)
        painter.drawRect((w / 2) - 75, h - 20, 150, 20);

        // Poutre Gauche (Ouest)
        painter.drawRect(0, (h / 2) - 75, 20, 150);

        // Poutre Droite (Est)
        painter.drawRect(w - 20, (h / 2) - 75, 20, 150);

        painter.restore();
    }

    // DESSIN DES OBSTACLES (Tonneaux, rochers...)
    painter.setPen(QPen(Qt::black, 4));
    for (const ObstacleModel* obstacle : roomModel->getItsObstacles()) {
        obstacleView.draw(painter, obstacle);
    }
}