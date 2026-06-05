#ifndef ENEMYMODEL_H
#define ENEMYMODEL_H

#include "entitymodel.h"
#include "qtimer.h"

class EnemyModel : public EntityModel
{
protected:
    float itsAttackCooldown;
    int itsAttackDamage;
    QTimer * IframeTimer;
    bool isInIframe;
public:
    EnemyModel();
    ~EnemyModel() = default;
    virtual void attack(QPointF heroPos) = 0;
    virtual bool handleAI(QPointF heroPos) = 0;
};

#endif // ENEMYMODEL_H
