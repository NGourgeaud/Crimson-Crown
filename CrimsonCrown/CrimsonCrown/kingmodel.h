#ifndef KINGMODEL_H
#define KINGMODEL_H

#include <QObject>
#include "bossmodel.h"
#include "hitzonemodel.h"

class KingModel : public QObject, public BossModel
{
    Q_OBJECT
public:
    KingModel(int maxHp, int speed, int x, int y, float attack1Cooldown, float attack2Cooldown, int attack1Damage, int attack2Damage);
    void update(QPointF heroPos);
    void takeDamage(int amount);
    void move(QPointF pos);
    QRectF getHitBox() const;

private:
    QTimer * itsCoolDownTimer1;
    QTimer * itsCoolDownTimer2;
    QTimer * itsSummoningTimer;
    QTimer * itsjumpingTimer;
    QTimer * itsAttack2Step2Timer;
    bool canAttack1;
    bool canAttack2;
    bool handleAI(QPointF heroPos);
    void attack1(QPointF heroPos);
    void attack2(QPointF heroPos);
    bool isSummoning;
    bool isJumping;

signals:
    void death(BossModel * Boss);
    void spawnEnemies();
    void attacking(HitZoneModel hitbox, int damage);

private slots:
    void finishSummoning();
    void attack2Step1();
    void attack2Step2();
    void resetAttack1Cooldown();
    void resetAttack2Cooldown();
    void resetIframe();
};

#endif // KINGMODEL_H
