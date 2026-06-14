// File Name: projectilemodel.h
// Group: A11
// Description: Header file defining the model for projectiles, managing their trajectory and collisions.

#ifndef PROJECTILEMODEL_H
#define PROJECTILEMODEL_H
#include "QRect"
#include "config.h"
#include "qline.h"
#include "qobject.h"
#include <QTimer>


class ProjectileModel : public QObject
{
    Q_OBJECT
private:
    QRectF itsHitbox;
    QLineF itsDiretion;
    int itsSpeed;
    int itsDamage;
    Team itsTeam;
    ProjectileType itsType;

    bool isInvincible;
    QTimer * itsInvinciblePeriode;


public:
    ///
    /// \brief Constructs a projectile with specified attributes.
    /// \param Hitbox The initial bounding box.
    /// \param direction The trajectory angle.
    /// \param speed The movement speed per tick.
    /// \param damage The damage dealt on impact.
    /// \param team The team firing the projectile.
    /// \param type The specific type of the projectile.
    ///
    ProjectileModel(QRectF Hitbox, QLineF direction, int speed, int damage, Team team, ProjectileType type);

    /// \brief Retrieves the team associated with the projectile.
    /// \return The Team enumerator.
    Team getItsTeam() const;

    /// \brief Retrieves the damage value.
    /// \return The damage amount.
    int getItsDamage() const;

    /// \brief Retrieves the movement direction line.
    /// \return The directional vector.
    QLineF getItsDiretion() const;

    /// \brief Updates the projectile's position.
    void update();

    /// \brief Retrieves the bounding box.
    /// \return The hitbox.
    QRectF getItsHitbox() const;

    /// \brief Retrieves the specific type of the projectile.
    /// \return The ProjectileType enumerator.
    ProjectileType getItsType() const;

    /// \brief Checks if the projectile is currently invincible (ignores walls briefly).
    /// \return True if invincible.
    bool getIsInvincible() const;

signals:
    /// \brief Emitted on each movement step.
    /// \param projectile Pointer to this instance.
    void tick(ProjectileModel* projectile);

private slots:
    /// \brief Resets the invincibility timer.
    void resetTimer(); };

#endif // PROJECTILEMODEL_H
