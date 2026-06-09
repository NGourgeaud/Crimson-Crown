#ifndef HUNTERMODEL_H
#define HUNTERMODEL_H

#include "playermodel.h"
#include "projectilemodel.h"
#include <QPolygon>
#include <QLineF>
#include <QTransform>
#include <QTimer>

class HunterModel : public PlayerModel
{
    Q_OBJECT
public:
    HunterModel();
    void attack1();
    void attack2();

signals:
    void attackPlayer(QPolygon attackHitBox, int damage);
    void fireProjectile(ProjectileModel* projectile);
};

#endif // HUNTERMODEL_H
