#ifndef OBSTACLEMODEL_H
#define OBSTACLEMODEL_H

#include <QRect>

class ObstacleModel
{
private:
    QRect itsHitBox;

public:
    ObstacleModel(int x, int y, int width, int height);

    // Getter
    QRect getHitBox() const;
};

#endif // OBSTACLEMODEL_H