#ifndef KNIGHTMODEL_H
#define KNIGHTMODEL_H

#include <QObject>
#include "enemymodel.h"
#include "QPolygon"

class KnightModel : public QObject, public EnemyModel
{
    Q_OBJECT
private:
    QTimer * itsCoolDownTimer;
    bool canAttack;
    bool handleAI(QPointF heroPos);

public:
    KnightModel(int maxHp, int speed, int x, int y, float attackCooldown, int attackDamage);
    void update(QPointF heroPos);
    void attack(QPointF heroPos);
    void takeDamage(int amount);
    void move(QPointF pos);
    QRectF getHitBox() const;

signals:
    void death(KnightModel * model);
    void attacking(QPolygon hitbox, int damage);

private slots:
    void resetAttackCooldown();
    void resetIframe();
};

#endif // KNIGHTMODEL_H
