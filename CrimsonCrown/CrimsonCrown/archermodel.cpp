#include "archermodel.h"
#include "qline.h"
#include <random>


ArcherModel::ArcherModel(int maxHp, int speed, int x, int y, float attackCooldown, int attackDamage)
{
    itsHP = maxHp;
    itsMaxHP = maxHp;
    itsSpeed = speed;
    itsHitBox = QRectF(x,y,LancerWidth, LancerHeight);
    itsAttackCooldown = attackCooldown;
    itsAttackDamage = attackDamage;
    canAttack = false;
    itsCoolDownTimer = new QTimer();
    IframeTimer = new QTimer();
    isInIframe = false;
    itsType = ARCHER;
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> firstShotTime(1,2);

    connect(itsCoolDownTimer, &QTimer::timeout, this, &ArcherModel::resetAttackCooldown);
    connect(IframeTimer, &QTimer::timeout, this, &ArcherModel::resetIframe);
    itsCoolDownTimer->start(firstShotTime(eng)*1000);
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
    itsCoolDownTimer->start(ArcherDefaultAttackCoolDown*1000);
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
    itsHitBox.moveTo(pos);
}

QRectF ArcherModel::getHitBox() const
{
    return itsHitBox;
}

// TODO
bool ArcherModel::handleAI(QPointF heroPos)
{
    // Goes away the player if it's too close
    for (int i = 0; i<itsSpeed; i++){
        if (QLineF(heroPos, itsHitBox.center()).length() < 200){

            QLineF aimLine = QLineF(itsHitBox.center(), heroPos);
            qreal angle = aimLine.angle();
            QLineF movementVector = QLineF::fromPolar(-1, angle);
            itsHitBox.translate(movementVector.dx(), movementVector.dy());

            if (i == itsSpeed-1)
                return false;
        }
    }
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
