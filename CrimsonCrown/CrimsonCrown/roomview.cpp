#include "roomview.h"
#include <QBrush>
#include <QColor>
#include <QPen>

RoomView::RoomView()
{
    spawnTexture = QPixmap(":/sol-spawn.png");

    combatTexture = QPixmap(":/sol-combat1.png");
}


void RoomView::draw(QPainter &painter, const RoomModel *roomModel, ObstacleView &obstacleView, int level)
{
    if (!roomModel) return;

    RoomType type = roomModel->getType();

    // --- 1. DESSIN DU SOL ---
    if  (type == Spawn){
        if (!spawnTexture.isNull()) {
            painter.drawPixmap(0, 0, roomModel->getWidth(), roomModel->getHeight(), spawnTexture);
        } else {
            // Sécurité si l'image du spawn bug : on affiche un carré bleu
            painter.fillRect(0, 0, roomModel->getWidth(), roomModel->getHeight(), QColor(50, 100, 150));
        }
    }
    else {
        // LOGIQUE DE NIVEAU
        if (level == 1) {
            // Si c'est une salle de combat ou un couloir, on met ton nouveau design
            if (type == Combat || type == CorridorHorizontal || type == CorridorVertical) {
                if (!combatTexture.isNull()) {
                    painter.drawPixmap(0, 0, roomModel->getWidth(), roomModel->getHeight(), combatTexture);
                } else {
                    // Sécurité si l'image ne charge pas
                    painter.setBrush(QBrush(QColor(64, 64, 64)));
                    painter.drawRect(0, 0, roomModel->getWidth(), roomModel->getHeight());
                }
            }
            else if (type == Boss) {
                painter.setBrush(QBrush(QColor(80, 20, 20)));
                painter.drawRect(0, 0, roomModel->getWidth(), roomModel->getHeight());
            }
        }
        else {
            // Niveau 2 (pas encore de textures, on garde les couleurs unies)
            if (type == Boss) {
                painter.setBrush(QBrush(QColor(80, 20, 20)));
            } else {
                painter.setBrush(QBrush(QColor(64, 64, 64)));
            }
            painter.drawRect(0, 0, roomModel->getWidth(), roomModel->getHeight());
        }
    }

    // --- 2. BORDURES ET PORTES ---
    painter.setBrush(Qt::NoBrush); // Important pour ne pas repeindre sur le sol
    painter.setPen(QPen(Qt::black, 4));
    painter.drawRect(0, 0, roomModel->getWidth(), roomModel->getHeight());

    if (roomModel->getIsLocked()) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(QColor(139, 69, 19), 12));
        painter.drawRect(6, 6, roomModel->getWidth() - 12, roomModel->getHeight() - 12);
    }

    // --- 3. OBSTACLES ---
    painter.setPen(QPen(Qt::black, 4));
    for (const ObstacleModel* obstacle : roomModel->getItsObstacles()) {
        // N'oublie pas qu'on avait aussi modifié draw() de l'obstacle pour prendre 'level' !
        // Si tu n'as pas encore fait cette modification, retire ", level" à la ligne suivante.
        obstacleView.draw(painter, obstacle, level);
    }
}
