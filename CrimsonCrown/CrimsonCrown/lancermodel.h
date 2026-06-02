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
    LancerModel(int maxHp, int speed, int x, int y, float attackCooldown, int maxAttackDistance, int attackDamage);
    void update(int heroX, int heroY);
    void attack(int heroX, int heroY);
    void takeDamage(int amount);
    void move(int axisX,int axisY);
    QRect getHitBox() const;

private:
    QTimer * itsCoolDownTimer;
    bool canAttack;
    bool handleAI(int heroX, int heroY);

signals:
    void death();
    void attacking(QPolygon hitbox);

private slots:
    void resetAttackCooldown();
};

#endif // LANCERMODEL_H
