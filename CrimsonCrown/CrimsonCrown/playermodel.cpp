#include "playermodel.h"

HeroType PlayerModel::getItsName() const
{
    return itsName;
}

PlayerModel::PlayerModel(int aHp, int aMaxHP, int aSpeed, int aAttack1Damage, int aAttack2Damage, HeroType aName)
    :itsDirection(Right), itsHitBox(QRectF(0, 0, playerWidth, playerHeight))
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
    IframeTimer = new QTimer();
    isInIframe = false;

    itsHP = aHp;
    itsMaxHP = aMaxHP;
    itsSpeed = aSpeed;
    itsAttack1Damage = aAttack1Damage;
    itsAttack2Damage = aAttack2Damage;
    itsName = aName;

    connect(itsCooldown1, &QTimer::timeout, this, &PlayerModel::resetAttackCooldown1);
    connect(itsCooldown2, &QTimer::timeout, this, &PlayerModel::resetAttackCooldown2);
    connect(IframeTimer, &QTimer::timeout, this, &PlayerModel::resetIframe);
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

void PlayerModel::takeDamage(int amont)
{
    if (!isInIframe){
        itsHP -= amont;
        isInIframe = true;
        IframeTimer->start(playerIframe*1000);
    }
    if (itsHP<=0)
        emit death();
}

void PlayerModel::move(QPointF pos)
{
    itsHitBox.moveTopLeft(pos);
}

void PlayerModel::resetAttackCooldown1()
{
    canAttack1 = true;
    itsCooldown1->stop();
}

void PlayerModel::resetAttackCooldown2()
{
    canAttack2 = true;
    itsCooldown2->stop();
}

void PlayerModel::resetIframe()
{
    isInIframe = false;
    IframeTimer->stop();
}

QRectF PlayerModel::getHitBox() const
{
    return itsHitBox;
}

void PlayerModel::setCenter(QPoint centre)
{
    itsHitBox.moveCenter(centre);
}

int PlayerModel::getHP()
{
    return itsHP;
}

int PlayerModel::getMaxHP()
{
    return itsMaxHP;
}

HeroType PlayerModel::getItsName() const
{
    return itsName;
}


void PlayerModel::regen()
{
    if(itsHP + playerRegen > itsMaxHP) {
        itsHP = itsMaxHP;
    }
    else {
        itsHP += playerRegen;
    }
}



