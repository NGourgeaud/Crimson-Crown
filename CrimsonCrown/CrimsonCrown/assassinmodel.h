#ifndef ASSASSINMODEL_H
#define ASSASSINMODEL_H

#include "hitzonemodel.h"
#include "playermodel.h"
#include "projectilemodel.h"
#include <QPolygon>
#include <QLineF>
#include <QTransform>

class AssassinModel : public PlayerModel
{
    Q_OBJECT
public:
    AssassinModel();
    void attack1() override;
    void attack2() override;

    bool getIsInvisible() const;

private:
    QTimer* smokeBombTimer; // Gère les 5 secondes
    bool isInvisible;

private slots:
    void resetInvisibility();

signals:
    void attackPlayer(HitZoneModel attackHitBox, int damage);
    void fireProjectile(ProjectileModel* projectile);
};

#endif // ASSASSINMODEL_H
