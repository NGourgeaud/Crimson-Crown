#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "entitymodel.h"
#include "config.h"

class PlayerModel : public EntityModel
{
protected:
    bool isMovingUp;
    bool isMovingDown;
    bool isMovingLeft;
    bool isMovingRight;

    Direction itsDirection;
    QRect itsHitBox;

public:
    PlayerModel();
    virtual void attack1() = 0;
    virtual void attack2() = 0;
    virtual void update();
    virtual void setMovementLeft(bool enabled);
    virtual void setMovementRight(bool enabled);
    virtual void setMovementUp(bool enabled);
    virtual void setMovementDown(bool enabled);
};

#endif // PLAYERMODEL_H
