#ifndef LANCERMODEL_H
#define LANCERMODEL_H

#include "enemymodel.h"
#include "qobject.h"
#include <QPolygon>
#include <QTimer>

class LancerModel : public QObject, public EnemyModel
{
    Q_OBJECT

public:
    LancerModel(int maxHp, int speed, int x, int y, float attackCooldown, int attackDamage);
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
    void death(LancerModel * model);
    void attacking(QPolygon hitbox, int damage);

private slots:
    void resetAttackCooldown();
};

#endif // LANCERMODEL_H
