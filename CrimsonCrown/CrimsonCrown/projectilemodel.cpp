
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

ProjectileType ProjectileModel::getItsType() const
{
    return itsType;
}

void ProjectileModel::resetTimer()
{
    isInvincible = false;
    itsInvinciblePeriode->stop();
}

bool ProjectileModel::getIsInvincible() const
{
    return isInvincible;
}

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
    itsInvinciblePeriode->start(3*100);
    connect(itsInvinciblePeriode, &QTimer::timeout, this, &ProjectileModel::resetTimer);
}

QLineF ProjectileModel::getItsDiretion() const
{
    return itsDiretion;
}
