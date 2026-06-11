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
    ProjectileModel(QRectF Hitbox, QLineF direction, int speed, int damage, Team team, ProjectileType type);
    Team getItsTeam() const;
    int getItsDamage() const;
    QLineF getItsDiretion() const;
    void update();
    QRectF getItsHitbox() const;
    ProjectileType getItsType() const;

    bool getIsInvincible() const;

signals:
    void tick(ProjectileModel*);

private slots:
    void resetTimer();
};

#endif // PROJECTILEMODEL_H
