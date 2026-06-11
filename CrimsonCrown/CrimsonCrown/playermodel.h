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
    QTimer * IframeTimer;
    bool isInIframe;

    Direction itsDirection;
    QRectF itsHitBox;

    bool hasSecretKey = false;

    HeroType itsName;

public:
    PlayerModel(int aHp, int aMaxHP, int aSpeed, int aAttack1Damage, int aAttack2Damage, HeroType aName);
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
    void move(QPointF pos);
    QRectF getHitBox() const;
    void setCenter(QPoint centre);

    // Getters for the caméra
    bool getIsMovingUp() const { return isMovingUp; }
    bool getIsMovingDown() const { return isMovingDown; }
    bool getIsMovingLeft() const { return isMovingLeft; }
    bool getIsMovingRight() const { return isMovingRight; }

    // getters pour life
    int getHP();
    int getMaxHP();

    void regen();

    bool getCanAttack1() const;
    bool getCanAttack2() const;
    int getCooldown1Remaining() const;
    int getCooldown2Remaining() const;

    HeroType getItsName() const;

    bool getHasSecretKey() const { return hasSecretKey; }
    void setHasSecretKey(bool hasKey) { hasSecretKey = hasKey; }

    void heal(int amount);

    void increaseDamage(double percentage);

signals:
    void death();

private slots:
    void resetAttackCooldown1();
    void resetAttackCooldown2();
    void resetIframe();
};

#endif // PLAYERMODEL_H
