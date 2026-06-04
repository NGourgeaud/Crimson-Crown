#ifndef LORDMODEL_H
#define LORDMODEL_H

#include <QObject>
#include "bossmodel.h"
#include <QPolygon>
#include "qline.h"
#include "qtimer.h"

class LordModel : public QObject, public BossModel
{
    Q_OBJECT
public:
    LordModel(int maxHp, int speed, int x, int y, float attack1Cooldown,float attack2Cooldown, int attack1Damage, int attack2Damage);
    void update(QPointF heroPos);
    void takeDamage(int amount);
    void move(QPointF pos);
    QRectF getHitBox() const;

private:
    QTimer * itsCoolDownTimer1;
    QTimer * itsCoolDownTimer2;
    bool canAttack1;
    bool canAttack2;
    bool handleAI(QPointF heroPos);
    void attack1(QPointF heroPos);
    void attack2(QPointF heroPos);
    // variables pour le dash
    int currentDashStep;
    int maxDashSteps;
    bool isDashing;
    QTimer * itsDashTimer;
    QLineF currentAimLine;

signals:
    void death(BossModel * Boss);
    void attacking(QPolygon hitbox, int damage);

private slots:
    void resetAttack1Cooldown();
    void resetAttack2Cooldown();
    void dashStep();
};

#endif // LORDMODEL_H
