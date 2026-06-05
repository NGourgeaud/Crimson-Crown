#ifndef ROOMVIEW_H
#define ROOMVIEW_H

#include <QPainter>
#include "roommodel.h"
#include "obstacleview.h"

class RoomView
{
public:
    RoomView();

    // Draw the entire room (floor, doors) and its internal obstacles
    void draw(QPainter &painter, const RoomModel *roomModel, ObstacleView &obstacleView);
};

#endif // ROOMVIEW_H
