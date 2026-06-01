#include "lancermodel.h"
#include "config.h"



LancerModel::LancerModel(int maxHp, int speed, int x, int y, float attackCooldown, int maxAttackDistance, int attackDamage) : EnemyModel()
{
    itsHP = maxHp;
    itsMaxHP = maxHp;
    itsSpeed = speed;
    itsX = x;
    itsY = y;
    itsHitBox = QRect(x,y,LancerWidth, LancerHeight);
    itsAttackCooldown = attackCooldown;
    itsMaxAttackDistance = maxAttackDistance;
    itsAttackDamage = attackDamage;
}

void LancerModel::update()
{

}

void LancerModel::attack()
{

}

void LancerModel::handleAI(int heroX, int heroY)
{

}

void LancerModel::takeDamage(int amount)
{

}

void LancerModel::move(int axisX, int axisY)
{

}

QRect LancerModel::getHitBox()
{
    return itsHitBox;
}
