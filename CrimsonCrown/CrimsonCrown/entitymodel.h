#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H
#include <QRect>

enum Direction{
    FacingLeft,
    FacingRight,
    FacingUp,
    FacingDown
};

class EntityModel
{
protected:
    int itsHP;
    int itsMaxHP;
    int itsSpeed;
    QRect itsHitBox;

public:
    EntityModel();
    virtual void update() = 0;
    virtual void takeDamage(int amount) = 0;
    virtual void move(int axisX,int axisY) = 0;
    virtual QRect getHitBox() const = 0;
};

#endif // ENTITYMODEL_H
