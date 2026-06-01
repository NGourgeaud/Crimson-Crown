#ifndef OBSTACLEVIEW_H
#define OBSTACLEVIEW_H

#include <QPainter>
#include "obstaclemodel.h"

class ObstacleView
{
public:
    ObstacleView();

    // elle dessine l'obstacle en fonction de son model
    void draw(QPainter &painter, const ObstacleModel *model);
};

#endif // OBSTACLEVIEW_H