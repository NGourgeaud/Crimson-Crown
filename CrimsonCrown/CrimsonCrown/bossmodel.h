#ifndef BOSSMODEL_H
#define BOSSMODEL_H

#include "config.h"
#include "entitymodel.h"
#include "qtimer.h"

class BossModel : public EntityModel
{
protected:
    float itsAttack1Cooldown;
    float itsAttack2Cooldown;
    int itsAttack1Damage;
    int itsAttack2Damage;
    QTimer * IframeTimer;
    bool isInIframe;
    BossType itsType;
public:
    BossModel();
    virtual ~BossModel();

    virtual void attack1(QPointF heroPos) = 0;
    virtual void attack2(QPointF heroPos) = 0;
    virtual void update(QPointF heroPos) = 0;
    BossType getItsType() const;
};

#endif // BOSSMODEL_H
