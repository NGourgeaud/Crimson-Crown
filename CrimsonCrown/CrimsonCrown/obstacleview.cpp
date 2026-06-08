#include "obstacleview.h"
#include <QBrush>
#include <QColor>

ObstacleView::ObstacleView()
{

}

void ObstacleView::draw(QPainter &painter, const ObstacleModel *model)
{

    QRect rect = model->getHitBox();

    painter.setBrush(QBrush(QColor(139, 69, 19)));

    painter.drawRect(rect);
}
