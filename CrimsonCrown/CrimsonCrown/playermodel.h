#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "entitymodel.h"
#include "config.h"
#include <QObject>
#include <QTimer>

class PlayerModel : public QObject, public EntityModel
{

    Q_OBJECT

protected:
    bool isMovingUp;
    bool isMovingDown;
    bool isMovingLeft;
    bool isMovingRight;

    bool isAttack1;
    bool isAttack2;
    QTimer * itsCooldown1;
    QTimer * itsCooldown2;
    bool canAttack1;
    bool canAttack2;
    int itsAttack1Damage;
    int itsAttack2Damage;

    Direction itsDirection;
    QRectF itsHitBox;

    //HeroType itsName;

public:
    PlayerModel(int aHp, int aMaxHP, int aSpeed, int aAttack1Damage, int aAttack2Damage, std::string aName);
    virtual void attack1() = 0;
    virtual void attack2() = 0;
    void update();
    void setMovementLeft(bool enabled);
    void setMovementRight(bool enabled);
    void setMovementUp(bool enabled);
    void setMovementDown(bool enabled);
    void setAttack1(bool enabled);
    void setAttack2(bool enabled);
    void takeDamage(int amont);
    void move(int axisX, int axisY);
    QRect getHitBox() const;
    void setCenter(QPoint centre);

signals:
    void death();

private slots:
    void resetAttackCooldown1();
    void resetAttackCooldown2();
};

#endif // PLAYERMODEL_H
