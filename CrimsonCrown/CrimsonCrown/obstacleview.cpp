#include "obstacleview.h"
#include <QBrush>
#include <QColor>

ObstacleView::ObstacleView()
{

    obsOubliettes = QPixmap(":/obstacle-tonneau.png");
}

void ObstacleView::draw(QPainter &painter, const ObstacleModel *model, int level)
{
    QRect rect = model->getHitBox();

    // Pour l'instant, on garde le carré marron par défaut
    painter.setBrush(QBrush(QColor(139, 69, 19)));
    painter.drawRect(rect);

}