#include "lordmodel.h"
#include "config.h"


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

    connect(itsCoolDownTimer1, &QTimer::timeout, this, &LordModel::resetAttack1Cooldown);
    connect(itsCoolDownTimer2, &QTimer::timeout, this, &LordModel::resetAttack2Cooldown);
}


void LordModel::update(int heroX, int heroY)
{

}

void LordModel::attack1(int heroX, int heroY)
{

}

void LordModel::attack2(int heroX, int heroY)
{

}

void LordModel::takeDamage(int amount)
{

}

void LordModel::move(int axisX, int axisY)
{

}

QRect LordModel::getHitBox() const
{

}

bool LordModel::handleAI(int heroX, int heroY)
{

}

void LordModel::resetAttack1Cooldown()
{

}

void LordModel::resetAttack2Cooldown()
{

}
