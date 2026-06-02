#ifndef LORDMODEL_H
#define LORDMODEL_H

#include <QObject>
#include "bossmodel.h"
#include "qtimer.h"

class LordModel : public BossModel
{
    Q_OBJECT
public:
    LordModel(int maxHp, int speed, int x, int y, float attack1Cooldown,float attack2Cooldown, int attack1Damage, int attack2Damage);
    void update(int heroX, int heroY);
    void attack1(int heroX, int heroY);
    void attack2(int heroX, int heroY);
    void takeDamage(int amount);
    void move(int axisX,int axisY);
    QRect getHitBox() const;

private:
    QTimer * itsCoolDownTimer1;
    QTimer * itsCoolDownTimer2;
    bool canAttack1;
    bool canAttack2;
    bool handleAI(int heroX, int heroY);

signals:
    void death(BossModel);
    void attacking(QPolygon hitbox, int damage);

private slots:
    void resetAttack1Cooldown();
    void resetAttack2Cooldown();
};

#endif // LORDMODEL_H
