#include "playermodel.h"
#include "config.h"

PlayerModel::PlayerModel()
    :itsHitBox(QRect(playerWidth,playerHeight,0,0))
{}

void PlayerModel::update()
{
    if(isMovingLeft){
        itsHitBox.translate(-itsSpeed,0);
        itsFacingLeft = true;
    }
    else if(isMovingRight) {
        itsHitBox.translate(itsSpeed,0);
        itsFacingRight = true;
    }

    if(isMovingDown){
        itsHitBox.translate(0,-itsSpeed);
        itsFacingDown = true;
    }
    else if(isMovingUp) {
        itsHitBox.translate(0,itsSpeed);
        itsFacingUp = true;
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
