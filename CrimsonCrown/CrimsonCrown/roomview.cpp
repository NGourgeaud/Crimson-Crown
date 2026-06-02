#include "roomview.h"
#include <QBrush>
#include <QColor>
#include <QPen>

RoomView::RoomView()
{
    // Emplacement pour charger les textures d'arrière-plan plus tard
}

void RoomView::draw(QPainter &painter, const RoomModel *roomModel, ObstacleView &obstacleView)
{
    if (!roomModel) return;

    // 1. DESSIN DU SOL (Zone de jeu active)
    // salle de base
    QRect roomRect(100, 150, 800, 450); // exemple
    painter.setBrush(QBrush(QColor(64, 64, 64))); // Gris foncé
    painter.setPen(QPen(Qt::black, 4)); // Bordures de la salle
    painter.drawRect(roomRect);

    // 2. DESSIN DES OBSTACLES
    for (const ObstacleModel* obstacle : roomModel->getItsObstacles()) {
        obstacleView.draw(painter, obstacle);
    }

    // 3. DESSIN DES BARRIÈRES / PORTES (Feedback visuel du GDD)
    // Si la salle est verrouillée, on affiche des blocs rouges pour bloquer les sorties
    if (roomModel->getIsLocked()) {
        painter.setBrush(QBrush(QColor(220, 20, 60))); // Rouge barrière
        painter.setPen(Qt::NoPen);

        // Exemple : Position des portes sur la maquette (gauche et bas)
        QRect porteGauche(80, 325, 20, 100); // a modifier pour les bon emplacement
        QRect porteBas(450, 600, 100, 20);  // a modifier pour les bon emplacement
        QRect porteDroite(80, 325, 20, 100);  // a modifier pour les bon emplacement
        QRect porteHaut(450, 600, 100, 20);  // a modifier pour les bon emplacement

        painter.drawRect(porteGauche);
        painter.drawRect(porteBas);
        painter.drawRect(porteDroite);
        painter.drawRect(porteHaut);
    }
}