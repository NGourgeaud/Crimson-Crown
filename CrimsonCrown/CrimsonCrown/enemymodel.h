#ifndef ENEMYMODEL_H
#define ENEMYMODEL_H

#include "config.h"
#include "entitymodel.h"
#include "qtimer.h"

class EnemyModel : public EntityModel
{
protected:
    float itsAttackCooldown;
    int itsAttackDamage;
    QTimer * IframeTimer;
    bool isInIframe;
    EnemyType itsType;
public:
    EnemyModel();
    ~EnemyModel() = default;
    virtual void attack(QPointF heroPos) = 0;
    virtual bool handleAI(QPointF heroPos) = 0;
    EnemyType getitsEnemyType();
    float getitsSpeed();
};

#endif // ENEMYMODEL_H
