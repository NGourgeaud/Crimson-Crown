// File Name: obstacleview.h
// Group: A11
// Description: Header file defining the visual rendering component for static obstacles.

#ifndef OBSTACLEVIEW_H
#define OBSTACLEVIEW_H

#include <QPainter>
#include <QPixmap>
#include "obstaclemodel.h"

class ObstacleView
{

private:

    QPixmap barrelTexture;

public:
    ///
    /// \brief Default constructor.
    ///
    ObstacleView();

    ///
    /// \brief Renders the obstacle on the screen.
    /// \param painter Reference to the QPainter object used for drawing.
    /// \param model Pointer to the ObstacleModel containing the hitbox data.
    ///
    void draw(QPainter &painter, const ObstacleModel *model);
};

#endif // OBSTACLEVIEW_H
