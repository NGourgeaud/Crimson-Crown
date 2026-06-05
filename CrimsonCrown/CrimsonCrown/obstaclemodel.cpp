#include "obstaclemodel.h"

ObstacleModel::ObstacleModel(int x, int y, int width, int height)
    : itsHitBox(x, y, width, height)
{

}

// getter for HitBox
QRect ObstacleModel::getHitBox() const
{
    return itsHitBox;
}
