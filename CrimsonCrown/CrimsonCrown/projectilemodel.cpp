#include "projectilemodel.h"

Team ProjectileModel::getItsTeam() const
{
    return itsTeam;
}

int ProjectileModel::getItsDamage() const
{
    return itsDamage;
}

void ProjectileModel::update()
{
    for (int i = 0; i<itsSpeed; i++){
       qreal angle = itsDiretion.angle();

        QLineF movementVector = QLineF::fromPolar(1, angle);

        itsHitbox.translate(movementVector.dx(), movementVector.dy());
        emit tick(this);
    }
}

QRectF ProjectileModel::getItsHitbox() const
{
    return itsHitbox;
}

ProjectileModel::ProjectileModel(QRectF Hitbox, QLineF direction, int speed, int damage, Team team)
{
    itsHitbox = Hitbox;
    itsDiretion = direction;
    itsSpeed = speed;
    itsDamage = damage;
    itsTeam = team;
}
