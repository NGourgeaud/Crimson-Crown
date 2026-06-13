// File Name: projectilemodel.cpp
// Group: A11
// Description: Model class for projectiles, handling movement trajectory, speed, and temporary invincibility.

#include "projectilemodel.h"

///
/// \brief Retrieves the team associated with this projectile.
/// \return The Team enumerator (e.g., PLAYER, ENEMY).
///
Team ProjectileModel::getItsTeam() const
{
    return itsTeam;
}

///
/// \brief Retrieves the damage value of the projectile.
/// \return An integer representing the damage dealt on impact.
///
int ProjectileModel::getItsDamage() const
{
    return itsDamage;
}

///
/// \brief Updates the projectile's position based on its speed and direction, emitting a tick for each step.
///
void ProjectileModel::update()
{
    for (int i = 0; i < itsSpeed; i++) {
        qreal angle = itsDiretion.angle();
        QLineF movementVector = QLineF::fromPolar(1, angle);

        itsHitbox.translate(movementVector.dx(), movementVector.dy());
        emit tick(this);
    }
}

///
/// \brief Retrieves the bounding box of the projectile.
/// \return A QRectF representing the projectile's physical area.
///
QRectF ProjectileModel::getItsHitbox() const
{
    return itsHitbox;
}

///
/// \brief Retrieves the specific type of the projectile.
/// \return The ProjectileType enumerator.
///
ProjectileType ProjectileModel::getItsType() const
{
    return itsType;
}

///
/// \brief Resets the invincibility timer, making the projectile vulnerable to destruction against walls.
///
void ProjectileModel::resetTimer()
{
    isInvincible = false;
    itsInvinciblePeriode->stop();
}

///
/// \brief Checks if the projectile is currently invincible (ignores obstacles briefly after spawning).
/// \return True if invincible, false otherwise.
///
bool ProjectileModel::getIsInvincible() const
{
    return isInvincible;
}

///
/// \brief Constructs a projectile with specified attributes.
/// \param Hitbox The initial bounding box.
/// \param direction The line representing the trajectory angle.
/// \param speed The movement speed per tick.
/// \param damage The damage dealt to targets.
/// \param team The team firing the projectile.
/// \param type The specific type of the projectile.
///
ProjectileModel::ProjectileModel(QRectF Hitbox, QLineF direction, int speed, int damage, Team team, ProjectileType type)
{
    itsHitbox = Hitbox;
    itsDiretion = direction;
    itsSpeed = speed;
    itsDamage = damage;
    itsTeam = team;
    itsType = type;

    isInvincible = true;
    itsInvinciblePeriode = new QTimer;
    itsInvinciblePeriode->start(3 * 100);
    connect(itsInvinciblePeriode, &QTimer::timeout, this, &ProjectileModel::resetTimer);
}

///
/// \brief Retrieves the movement direction line.
/// \return A QLineF representing the directional vector.
///
QLineF ProjectileModel::getItsDiretion() const
{
    return itsDiretion;
}
