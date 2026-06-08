#ifndef ROOMVIEW_H
#define ROOMVIEW_H

#include <QPainter>
#include <QPixmap>
#include "roommodel.h"
#include "obstacleview.h"

class RoomView
{
public:
    RoomView();

    // Dessine l'ensemble de la salle (sol, portes) et ses obstacles internes
    void draw(QPainter &painter, const RoomModel *roomModel, ObstacleView &obstacleView);
private:
    QPixmap spawnTexture;
};

#endif // ROOMVIEW_H