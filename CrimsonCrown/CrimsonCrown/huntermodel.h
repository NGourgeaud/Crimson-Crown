#ifndef HUNTERMODEL_H
#define HUNTERMODEL_H

#include "hitzonemodel.h"
#include "playermodel.h"
#include "projectilemodel.h"
#include <QPolygon>
#include <QLineF>
#include <QTransform>
#include <QTimer>

class HunterModel : public PlayerModel
{
    Q_OBJECT
private:
    QTimer * itsAttack2Timer;
    float itsAttack2Time;
    QPolygon itsAttack2Area;
public:
    HunterModel();
    void attack1();
    void attack2();

signals:
    void attackPlayer(HitZoneModel attackHitBox, int damage);
    void fireProjectile(ProjectileModel* projectile);

private slots:
    void tickAttack2();
};

#endif // HUNTERMODEL_H
