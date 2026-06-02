#ifndef BOSSMODEL_H
#define BOSSMODEL_H

#include "entitymodel.h"

class BossModel : public EntityModel
{
protected:
    float attack1Cooldown;
    float attack2Cooldown;
    int attack1Damage;
    int attack2Damage;
public:
    BossModel();
    virtual void attack1(int heroX, int heroY) = 0;
    virtual void attack2(int heroX, int heroY) = 0;
    virtual void handleAI(int heroX, int heroY) = 0;
};

#endif // BOSSMODEL_H
