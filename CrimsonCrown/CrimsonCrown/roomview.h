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

    void draw(QPainter &painter, const RoomModel *roomModel, ObstacleView &obstacleView, int level);

private:
    QPixmap spawnTexture;
    QPixmap combatTexture;
};

#endif // ROOMVIEW_H