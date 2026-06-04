#ifndef BOSSMODEL_H
#define BOSSMODEL_H

#include "entitymodel.h"

class BossModel : public EntityModel
{
protected:
    float itsAttack1Cooldown;
    float itsAttack2Cooldown;
    int itsAttack1Damage;
    int itsAttack2Damage;
public:
    BossModel();
    virtual ~BossModel();

    virtual void attack1(QPointF heroPos) = 0;
    virtual void attack2(QPointF heroPos) = 0;
    virtual bool handleAI(QPointF heroPos) = 0;
};

#endif // BOSSMODEL_H
