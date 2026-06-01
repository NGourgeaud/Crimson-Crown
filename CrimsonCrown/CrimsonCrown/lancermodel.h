#ifndef LANCERMODEL_H
#define LANCERMODEL_H

#include "enemymodel.h"

class LancerModel : public EnemyModel
{
public:
    LancerModel(int maxHp, int speed, int x, int y, float attackCooldown, int maxAttackDistance, int attackDamage);
    void update();
    void attack();
    void handleAI(int heroX, int heroY);
    void takeDamage(int amount);
    void move(int axisX,int axisY);
    QRect getHitBox();
};

#endif // LANCERMODEL_H
