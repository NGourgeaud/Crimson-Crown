#include "obstaclemodel.h"

// Implémentation du constructeur
// On utilise une liste d'initialisation pour attribuer les valeurs directement
ObstacleModel::ObstacleModel(int x, int y, int width, int height)
    : positionX(x), positionY(y), itsHitBox(x, y, width, height)
{

}

// getter pour X
int ObstacleModel::getX() const
{
    return positionX;
}

// getter pour Y
int ObstacleModel::getY() const
{
    return positionY;
}

// getter pour la HitBox
QRect ObstacleModel::getHitBox() const
{
    return itsHitBox;
}