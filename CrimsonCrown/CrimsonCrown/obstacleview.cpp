#include "obstacleview.h"
#include <QBrush>
#include <QColor>

ObstacleView::ObstacleView()
{
    // charger le sprite plus tard.
    // itsSpriteSheet = QPixmap(":/sprites/caisse.png");
}

void ObstacleView::draw(QPainter &painter, const ObstacleModel *model)
{

    // recuperation de la zone d'affichage / hitbox
    QRect rect = model->getHitBox();

    // marron
    painter.setBrush(QBrush(QColor(139, 69, 19)));

    // dessin du rectangle
    painter.drawRect(rect);
}