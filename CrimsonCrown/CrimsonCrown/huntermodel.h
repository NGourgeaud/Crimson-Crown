// File Name: huntermodel.h
// Group: A11
// Description: Header file defining the Hunter hero model, handling ranged attacks and traps.

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
    ///
    /// \brief Constructs the Hunter player model with specific stats.
    ///
    HunterModel();

    ///
    /// \brief Executes the primary ranged arrow attack.
    ///
    void attack1();

    ///
    /// \brief Deploys the secondary area-of-effect trap.
    ///
    void attack2();

signals:
    ///
    /// \brief Emitted when an area attack inflicts damage.
    /// \param attackHitBox The area of the attack.
    /// \param damage The damage to deal.
    ///
    void attackPlayer(HitZoneModel attackHitBox, int damage);

    ///
    /// \brief Emitted to fire an arrow projectile into the game.
    /// \param projectile Pointer to the generated projectile.
    ///
    void fireProjectile(ProjectileModel* projectile);

private slots:
    ///
    /// \brief Triggers periodic damage for the active trap.
    ///
    void tickAttack2();
};

#endif // HUNTERMODEL_H
