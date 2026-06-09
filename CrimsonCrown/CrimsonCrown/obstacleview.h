#ifndef OBSTACLEVIEW_H
#define OBSTACLEVIEW_H

#include <QPainter>
#include <QPixmap>
#include "obstaclemodel.h"

class ObstacleView
{
private:
    QPixmap obsOubliettes;

public:
    ObstacleView();

    void draw(QPainter &painter, const ObstacleModel *model, int level);
};

#endif // OBSTACLEVIEW_H