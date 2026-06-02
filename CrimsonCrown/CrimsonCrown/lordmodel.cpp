#include "lordmodel.h"
#include "config.h"
#include "qline.h"


LordModel::LordModel(int maxHp, int speed, int x, int y, float attack1Cooldown, float attack2Cooldown, int attack1Damage, int attack2Damage)
{
    itsHP = maxHp;
    itsMaxHP = maxHp;
    itsSpeed = speed;
    itsHitBox = QRect(x,y, LordWidth, LordHeight);
    itsAttack1Cooldown = attack1Cooldown;
    itsAttack2Cooldown = attack2Cooldown;
    itsAttack1Damage = attack1Damage;
    itsAttack2Damage = attack2Damage;
    itsCoolDownTimer1 = new QTimer();
    itsCoolDownTimer2 = new QTimer();
    canAttack1 = true;
    canAttack2 = true;

    connect(itsCoolDownTimer1, &QTimer::timeout, this, &LordModel::resetAttack1Cooldown);
    connect(itsCoolDownTimer2, &QTimer::timeout, this, &LordModel::resetAttack2Cooldown);
}


void LordModel::update(int heroX, int heroY)
{
    if(handleAI(heroX, heroY))
    {
        if(QLineF(QPoint(heroX, heroY), itsHitBox.center()).length() < LordAttack1HitBoxLenght and canAttack1){
            attack1(heroX, heroY);
        }
        else if (QLineF(QPoint(heroX, heroY), itsHitBox.center()).length() > LordAttack1HitBoxLenght-20 and canAttack2){
            attack2(heroX, heroY);
        }
    }
}

void LordModel::attack1(int heroX, int heroY)
{

}

void LordModel::attack2(int heroX, int heroY)
{

}

void LordModel::takeDamage(int amount)
{
    itsHP-= amount;
    if (itsHP<=0)
        emit death(this);
}

void LordModel::move(int axisX, int axisY)
{
    itsHitBox.setTopLeft(QPoint(axisX,axisY));
}

QRect LordModel::getHitBox() const
{
    return itsHitBox;
}

bool LordModel::handleAI(int heroX, int heroY)
{

}

void LordModel::resetAttack1Cooldown()
{
    canAttack1 = true;
    itsCoolDownTimer1->stop();
}

void LordModel::resetAttack2Cooldown()
{
    canAttack2 = true;
    itsCoolDownTimer2->stop();
}
