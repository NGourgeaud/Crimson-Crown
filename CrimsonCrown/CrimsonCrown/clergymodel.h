#ifndef CLERGYMODEL_H
#define CLERGYMODEL_H


#include "bossmodel.h"
#include "projectilemodel.h"
#include <QtCore/qline.h>
#include <QtCore/qtimer.h>

class clergyModel: public QObject, public BossModel
{
    Q_OBJECT
public:
    clergyModel(int maxHp, int speed, int x, int y, float attack1Cooldown,float attack2Cooldown, int attack1Damage, int attack2Damage);
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
    QLineF currentAimLine;

signals:
    void death(BossModel * Boss);
    void attacking(ProjectileModel * hitbox);

private slots:
    void resetAttack1Cooldown();
    void resetAttack2Cooldown();
    void resetIframe();
};

#endif // CLERGYMODEL_H
