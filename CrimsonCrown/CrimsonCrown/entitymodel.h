#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H
#include <QRect>
#include <QPointF>

enum Direction{
    Left,
    Right,
    Up,
    Down,
    LeftDown,
    LeftUp,
    RightDown,
    RightUp
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
    virtual ~EntityModel(); // FIX : On ajoute le destructeur virtuel !

    virtual void takeDamage(int amount) = 0;
    virtual void move(QPointF pos) = 0;
    virtual QRectF getHitBox() const = 0;
    int getItsHP() const;
};

#endif // ENTITYMODEL_H
