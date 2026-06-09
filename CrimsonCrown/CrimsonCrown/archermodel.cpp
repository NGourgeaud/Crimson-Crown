#include "archermodel.h"
#include "qline.h"


ArcherModel::ArcherModel(int maxHp, int speed, int x, int y, float attackCooldown, int attackDamage)
{
    itsHP = maxHp;
    itsMaxHP = maxHp;
    itsSpeed = speed;
    itsHitBox = QRectF(x,y,LancerWidth, LancerHeight);
    itsAttackCooldown = attackCooldown;
    itsAttackDamage = attackDamage;
    canAttack = true;
    itsCoolDownTimer = new QTimer();
    IframeTimer = new QTimer();
    isInIframe = false;
    itsType = ARCHER;

    connect(itsCoolDownTimer, &QTimer::timeout, this, &ArcherModel::resetAttackCooldown);
    connect(IframeTimer, &QTimer::timeout, this, &ArcherModel::resetIframe);
}

void ArcherModel::update(QPointF heroPos)
{
    if(handleAI(heroPos) and canAttack)
        attack(heroPos);
}

void ArcherModel::attack(QPointF heroPos)
{
    QLineF aimLine(itsHitBox.center(), heroPos);
    ProjectileModel * projectile = new ProjectileModel(QRectF(itsHitBox.center().x(), itsHitBox.center().y(), ArcherAttackHitboxWidth, ArcherAttackHitboxHeight), aimLine,
                                     3, ArcherDefaultAttackDamage, ENEMY, ARROW);
    emit attacking(projectile);
    canAttack = false;
    itsCoolDownTimer->start(ArcherDefaultAttackDamage*1000);
}

void ArcherModel::takeDamage(int amount)
{
    if (!isInIframe){
        itsHP-= amount;
        IframeTimer->start(LancerIframe*1000);
    }
    if (itsHP<=0)
        emit death(this);
}

void ArcherModel::move(QPointF pos)
{
    itsHitBox.setTopLeft(pos);
}

QRectF ArcherModel::getHitBox() const
{
    return itsHitBox;
}

// TODO
bool ArcherModel::handleAI(QPointF heroPos)
{
    return true;
}

void ArcherModel::resetAttackCooldown()
{
    canAttack = true;
    itsCoolDownTimer->stop();
}

void ArcherModel::resetIframe()
{
    IframeTimer->stop();
    isInIframe = false;
}
