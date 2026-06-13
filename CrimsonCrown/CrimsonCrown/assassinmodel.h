// File Name: assassinmodel.h
// Group: A11
// Description: Header file defining the Assassin hero character and its abilities.

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
    ///
    /// \brief Initializes the Assassin player character with its base stats.
    ///
    AssassinModel();

    ///
    /// \brief Executes the primary attack (throwing a knife).
    ///
    void attack1() override;

    ///
    /// \brief Executes the secondary ability (smoke bomb).
    ///
    void attack2() override;

    ///
    /// \brief Checks if the Assassin is currently invisible.
    /// \return True if invisible, false otherwise.
    ///
    bool getIsInvisible() const;

private:
    QTimer * smokeBombTimer; // Handles the 5 seconds duration
    bool isInvisible;

private slots:
    ///
    /// \brief Ends the invisibility effect.
    ///
    void resetInvisibility();

signals:
    ///
    /// \brief Emitted for melee attack collisions.
    /// \param attackHitBox The area of the attack.
    /// \param damage The damage to deal.
    ///
    void attackPlayer(HitZoneModel attackHitBox, int damage);

    ///
    /// \brief Emitted to spawn a knife projectile in the game world.
    /// \param projectile Pointer to the generated projectile.
    ///
    void fireProjectile(ProjectileModel* projectile);
};

#endif // ASSASSINMODEL_H
