// File Name: obstaclemodel.cpp
// Group: A11
// Description: Model class representing static obstacles within dungeon rooms, defining their physical hitboxes.

#include "obstaclemodel.h"

///
/// \brief Initializes an obstacle with a specific position and size.
/// \param x The starting X coordinate.
/// \param y The starting Y coordinate.
/// \param width The width of the obstacle.
/// \param height The height of the obstacle.
///
ObstacleModel::ObstacleModel(int x, int y, int width, int height)
    : itsHitBox(x, y, width, height)
{
}

///
/// \brief Retrieves the physical bounding box of the obstacle.
/// \return A QRect representing the obstacle's collision area.
///
QRect ObstacleModel::getHitBox() const
{
    return itsHitBox;
}
