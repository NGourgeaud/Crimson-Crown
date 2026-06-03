#include "playermodel.h"

PlayerModel::PlayerModel(int aHp, int aMaxHP, int aSpeed, int aAttack1Damage, int aAttack2Damage, HeroType aName)
    :itsDirection(Right), itsHitBox(QRect(0, 0, playerWidth, playerHeight))
{
    isMovingUp = false;
    isMovingDown = false;
    isMovingLeft = false;
    isMovingRight = false;
    isAttack1 = false;
    isAttack2 = false;
    itsCooldown1 = new QTimer();
    itsCooldown2 = new QTimer();
    canAttack1 = true;
    canAttack2 = true;

    itsHP = aHp;
    itsMaxHP = aMaxHP;
    itsSpeed = aSpeed;
    itsAttack1Damage = aAttack1Damage;
    itsAttack2Damage = aAttack2Damage;
    itsName = aName;

    connect(itsCooldown1, &QTimer::timeout, this, &PlayerModel::resetAttackCooldown1);
    connect(itsCooldown2, &QTimer::timeout, this, &PlayerModel::resetAttackCooldown2);
}

void PlayerModel::update()
{

    if(isMovingLeft && isMovingDown){
        itsHitBox.translate(-itsSpeed/2,itsSpeed/2);
        itsDirection = LeftDown;
    }
    else if(isMovingLeft && isMovingUp){
        itsHitBox.translate(-itsSpeed/2,-itsSpeed/2);
        itsDirection = LeftUp;
    }
    else if(isMovingRight && isMovingDown){
        itsHitBox.translate(itsSpeed/2,itsSpeed/2);
        itsDirection = RightDown;
    }
    else if(isMovingRight && isMovingUp){
        itsHitBox.translate(itsSpeed/2,-itsSpeed/2);
        itsDirection = RightUp;
    }
    else if(isMovingLeft){
        itsHitBox.translate(-itsSpeed,0);
        itsDirection = Left;
    }
    else if(isMovingRight) {
        itsHitBox.translate(itsSpeed,0);
        itsDirection = Right;
    }
    else if(isMovingDown){
        itsHitBox.translate(0,itsSpeed);
        itsDirection = Down;
    }
    else if(isMovingUp) {
        itsHitBox.translate(0,-itsSpeed);
        itsDirection = Up;
    }

    if(isAttack1){
        attack1();
    }
    else if(isAttack2){
        attack2();
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

void PlayerModel::setAttack1(bool enabled)
{
    isAttack1 = enabled;
}

void PlayerModel::setAttack2(bool enabled)
{
    isAttack2 = enabled;
}

void PlayerModel::take_damage(int amont)
{
    itsHP -= amont;
    if (itsHP<=0)
        emit death();
}

void PlayerModel::move(int axisX, int axisY)
{
    itsHitBox.setTopLeft(QPoint(axisX,axisY));
}

void PlayerModel::resetAttackCooldown1()
{
    isAttack1 = true;
    itsCooldown1->stop();
}

void PlayerModel::resetAttackCooldown2()
{
    isAttack2 = true;
    itsCooldown2->stop();
}

QRect PlayerModel::getItsHitBox() const
{
    return itsHitBox;
}
