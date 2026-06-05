#ifndef OBSTACLEVIEW_H
#define OBSTACLEVIEW_H

#include <QPainter>
#include <QPixmap>
#include "obstaclemodel.h"

class ObstacleView
{

private:

    // QPixmap itsSpriteSheet;
public:
    ObstacleView();

    void draw(QPainter &painter, const ObstacleModel *model);


};

#endif // OBSTACLEVIEW_H
