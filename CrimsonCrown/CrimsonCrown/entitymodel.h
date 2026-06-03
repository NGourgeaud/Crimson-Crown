#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H
#include <QRect>

enum Direction{
    Left,
    Right,
    Up,
    Down
};

class EntityModel
{
protected:
    int itsHP;
    int itsMaxHP;
    int itsSpeed;
    QRectF itsHitBox;

public:
    EntityModel();
    virtual void takeDamage(int amount) = 0;
    virtual void move(QPointF pos) = 0;
    virtual QRectF getHitBox() const = 0;
};

#endif // ENTITYMODEL_H
