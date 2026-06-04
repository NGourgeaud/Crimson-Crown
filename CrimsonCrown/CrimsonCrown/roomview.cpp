#include "roomview.h"
#include <QBrush>
#include <QColor>
#include <QPen>

RoomView::RoomView() {}

void RoomView::draw(QPainter &painter, const RoomModel *roomModel, ObstacleView &obstacleView)
{
    if (!roomModel) return;

    // 1. DESSIN DU SOL (Séparation Salles vs Couloirs)
    if (!roomModel->getIsCorridor()) {
        // --- C'est une vraie salle (Spawn, Combat, Boss) ---
        QPixmap sol(":/sol_donjon.png");

        // On utilise drawPixmap (et pas Tiled). L'image va s'étirer aux dimensions exactes de la salle.
        painter.drawPixmap(0, 0, roomModel->getWidth(), roomModel->getHeight(), sol);
    }
    else {
        // --- C'est un couloir ---
        // On met une couleur unie (un gris foncé pour faire "ombre" ou transition)
        painter.setBrush(QBrush(QColor(40, 40, 40)));
        painter.drawRect(0, 0, roomModel->getWidth(), roomModel->getHeight());
    }

    // 2. DESSIN DES MURS (Contours)
    painter.setBrush(Qt::NoBrush); // TRÈS IMPORTANT : On dit au pinceau de ne pas remplir l'intérieur, sinon ça cache l'image !
    painter.setPen(QPen(Qt::black, 4));
    painter.drawRect(0, 0, roomModel->getWidth(), roomModel->getHeight());

    // 3. EFFET VISUEL : PALISSADES EN BOIS (Quand la salle se verrouille)
    if (roomModel->getIsLocked()) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(QColor(139, 69, 19), 12));
        painter.drawRect(6, 6, roomModel->getWidth() - 12, roomModel->getHeight() - 12);
    }

    // 4. DESSIN DES OBSTACLES
    painter.setPen(QPen(Qt::black, 4));
    for (const ObstacleModel* obstacle : roomModel->getItsObstacles()) {
        obstacleView.draw(painter, obstacle);
    }
}