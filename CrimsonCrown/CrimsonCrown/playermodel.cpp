#include "playermodel.h"

PlayerModel::PlayerModel()
    :itsDirection(Right), itsHitBox(QRect(playerWidth,playerHeight,0,0))
{}

void PlayerModel::update()
{
    if(isMovingLeft){
        itsHitBox.translate(-itsSpeed,0);
        itsDirection = Left;
    }
    else if(isMovingRight) {
        itsHitBox.translate(itsSpeed,0);
        itsDirection = Right;
    }

    if(isMovingDown){
        itsHitBox.translate(0,-itsSpeed);
        itsDirection = Down;
    }
    else if(isMovingUp) {
        itsHitBox.translate(0,itsSpeed);
        itsDirection = Up;
    }
}

void PlayerModel::setMovementLeft(bool enabled)
{
    isMovingLeft = enabled;
}

void PlayerModel::setMovementUp(bool enabled)
{
    isMovingUp = enabled;
}

void PlayerModel::setMovementRight(bool enabled)
{
    isMovingRight = enabled;
}


void PlayerModel::setMovementDown(bool enabled)
{
    isMovingDown = enabled;
}
