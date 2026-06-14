// File Name: obstaclemodel.h
// Group: A11
// Description: Header file defining the model for static obstacles within dungeon rooms.

#ifndef OBSTACLEMODEL_H
#define OBSTACLEMODEL_H

#include <QRect>

class ObstacleModel
{
private:
    QRect itsHitBox;

public:
    ///
    /// \brief Initializes an obstacle with a specific position and size.
    /// \param x The starting X coordinate.
    /// \param y The starting Y coordinate.
    /// \param width The width of the obstacle.
    /// \param height The height of the obstacle.
    ///
    ObstacleModel(int x, int y, int width, int height);

    ///
    /// \brief Retrieves the physical bounding box of the obstacle.
    /// \return A QRect representing the collision area.
    ///
    QRect getHitBox() const;
};

#endif // OBSTACLEMODEL_H
