#ifndef OBSTACLEMODEL_H
#define OBSTACLEMODEL_H

#include <QRect>

class ObstacleModel
{
private:
    QRect itsHitBox;

public:
    ObstacleModel(int x, int y, int width, int height);

    // Getters
    int getX() const;
    int getY() const;
    QRect getHitBox() const;
};

#endif // OBSTACLEMODEL_H