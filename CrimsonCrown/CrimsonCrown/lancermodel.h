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
};

#endif // LANCERMODEL_H
