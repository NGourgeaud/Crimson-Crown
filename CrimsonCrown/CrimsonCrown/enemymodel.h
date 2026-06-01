#ifndef ENEMYMODEL_H
#define ENEMYMODEL_H

#include "entitymodel.h"

class EnemyModel : public EntityModel
{
protected:
    float itsAttackCooldown;
    int itsMaxAttackDistance;
    int itsAttackDamage;
public:
    EnemyModel();
    virtual void attack(int heroX, int heroY) = 0;
    virtual bool handleAI(int heroX, int heroY) =0;
};

#endif // ENEMYMODEL_H
