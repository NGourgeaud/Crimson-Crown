#ifndef OBSTACLEMODEL_H
#define OBSTACLEMODEL_H

#include <QRect>

class ObstacleModel
{
private:
    int positionX;
    int positionY;
    QRect itsHitBox;

public:
    // Un constructeur qui prend les dimensions en paramètres est plus pratique
    ObstacleModel(int x, int y, int width, int height);

    // Getters
    int getX() const;
    int getY() const;
    QRect getHitBox() const; // Attention aux parenthèses ici !
};

#endif // OBSTACLEMODEL_H