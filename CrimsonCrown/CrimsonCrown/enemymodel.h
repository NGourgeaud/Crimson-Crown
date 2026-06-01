#ifndef ENEMYMODEL_H
#define ENEMYMODEL_H

#include "entitymodel.h"

class EnemyModel : public EntityModel
{
protected:
    float attackCooldown;
    int maxAttackDistance;
    int attackDamage;
public:
    EnemyModel();
    virtual void attack() = 0;
    virtual void handleAI(int heroX, int heroY) =0;
};

#endif // ENEMYMODEL_H
