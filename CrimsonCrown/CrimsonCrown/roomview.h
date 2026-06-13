// File Name: roomview.h
// Group: A11
// Description: Header file defining the visual rendering component for a dungeon room.

#ifndef ROOMVIEW_H
#define ROOMVIEW_H

#include <QPainter>
#include <QPixmap>
#include "roommodel.h"
#include "obstacleview.h"

class RoomView
{
public:
    ///
    /// \brief Default constructor.
    ///
    RoomView();

    ///
    /// \brief Draws the entire room (floor, doors) and its internal obstacles.
    /// \param painter Reference to the QPainter object.
    /// \param roomModel Pointer to the RoomModel to render.
    /// \param obstacleView Reference to the ObstacleView used to draw internal obstacles.
    ///
    void draw(QPainter &painter, const RoomModel *roomModel, ObstacleView &obstacleView);
private:
    QPixmap spawnTexture;
};

#endif // ROOMVIEW_H
