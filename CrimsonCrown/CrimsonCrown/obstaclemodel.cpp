#include "obstaclemodel.h"

// Implémentation du constructeur
// On utilise une liste d'initialisation pour attribuer les valeurs directement
ObstacleModel::ObstacleModel(int x, int y, int width, int height)
    : itsHitBox(x, y, width, height)
{

}

// getter pour la HitBox
QRect ObstacleModel::getHitBox() const
{
    return itsHitBox;
}