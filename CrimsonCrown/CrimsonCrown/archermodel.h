#ifndef ARCHERMODEL_H
#define ARCHERMODEL_H

#include <QObject>
#include "enemymodel.h"
#include "projectilemodel.h"

class ArcherModel : public QObject, public EnemyModel
{
    Q_OBJECT
public:
    ArcherModel(int maxHp, int speed, int x, int y, float attackCooldown, int attackDamage);
    void update(QPointF heroPos);
    void attack(QPointF heroPos);
    void takeDamage(int amount);
    void move(QPointF pos);
    QRectF getHitBox() const;

private:
    QTimer * itsCoolDownTimer;
    bool canAttack;
    bool handleAI(QPointF heroPos);

signals:
    void death(ArcherModel * model);
    void attacking(ProjectileModel * projectile);

private slots:
    void resetAttackCooldown();
    void resetIframe();
};

#endif // ARCHERMODEL_H
