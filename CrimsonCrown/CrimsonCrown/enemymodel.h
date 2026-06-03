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
    ~EnemyModel() = default;
    virtual void attack(QPointF heroPos) = 0;
    virtual bool handleAI(QPointF heroPos) =0;
};

#endif // ENEMYMODEL_H
