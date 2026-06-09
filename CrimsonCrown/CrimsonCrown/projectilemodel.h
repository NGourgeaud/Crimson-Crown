#ifndef PROJECTILEMODEL_H
#define PROJECTILEMODEL_H
#include "QRect"
#include "config.h"
#include "qline.h"
#include "qobject.h"


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

public:
    ProjectileModel(QRectF Hitbox, QLineF direction, int speed, int damage, Team team, ProjectileType type);
    Team getItsTeam() const;
    int getItsDamage() const;
    QLineF getItsDiretion() const;
    void update();
    QRectF getItsHitbox() const;
    ProjectileType getItsType() const;

signals:
    void tick(ProjectileModel*);
};

#endif // PROJECTILEMODEL_H
