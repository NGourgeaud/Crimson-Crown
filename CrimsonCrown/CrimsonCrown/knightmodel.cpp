#include "knightmodel.h"
#include "QLineF"
#include "qtransform.h"

bool KnightModel::handleAI(QPointF heroPos)
{
    // Goes towards the player if it's too far
    if (QLineF(heroPos, itsHitBox.center()).length() > KnightAttackHitboxWidth-10){
        for (int i = 0; i<itsSpeed; i++){
            QLineF aimLine = QLineF(itsHitBox.center(), heroPos);
            qreal angle = aimLine.angle();

            QLineF movementVector = QLineF::fromPolar(1, angle);

            itsHitBox.translate(movementVector.dx(), movementVector.dy());
        }
        return false;
    }
    else{
        return true;
    }
}

KnightModel::KnightModel(int maxHp, int speed, int x, int y, float attackCooldown, int attackDamage)
{
    itsMaxHP = maxHp;
    itsHP = maxHp;
    itsSpeed = speed;
    itsHitBox = QRectF(x, y, KnightWidth, KnightHeight);
    itsAttackCooldown = attackCooldown;
    itsAttackDamage = attackDamage;
    itsCoolDownTimer = new QTimer();
    canAttack = false;
    IframeTimer = new QTimer();
    isInIframe = false;
    itsType = KNIGHT;

    connect(itsCoolDownTimer, &QTimer::timeout, this, &KnightModel::resetAttackCooldown);
    connect(IframeTimer, &QTimer::timeout, this, &KnightModel::resetIframe);
    itsCoolDownTimer->start(1500);
}

void KnightModel::update(QPointF heroPos)
{
    if(handleAI(heroPos) and canAttack)
        attack(heroPos);
}

void KnightModel::attack(QPointF heroPos)
{
    // 1. Find the angle between the lancer and the hero
    QLineF aimLine(itsHitBox.center(), heroPos);
    qreal angle = aimLine.angle(); // Returns the angle in degrees (0 is right, 90 is up)

    // 1. Create the attack rectangle centered on 0,0
    QRect baseAttackRect(0, -KnightAttackHitboxHeight / 2, KnightAttackHitboxWidth, KnightAttackHitboxHeight);

    // 2. Initialize the QTransform
    QTransform transform;

    // 3. Translate the center of the coordinate system towards your character
    transform.translate(itsHitBox.center().x(), itsHitBox.center().y());

    // 4. Apply rotation
    transform.rotate(-angle);

    // 5. Generates the final polygon
    QPolygon attackHitbox = transform.mapToPolygon(baseAttackRect);
    emit attacking(attackHitbox, itsAttackDamage);
    canAttack = false;
    itsCoolDownTimer->start(itsAttackCooldown*1000);
}

void KnightModel::takeDamage(int amount)
{
    if (!isInIframe){
        itsHP-= amount;
        IframeTimer->start(LancerIframe*1000);
    }
    if (itsHP<=0)
        emit death(this);
}

void KnightModel::move(QPointF pos)
{
    itsHitBox.setTopLeft(pos);
}

QRectF KnightModel::getHitBox() const
{
    return itsHitBox;
}

void KnightModel::resetAttackCooldown()
{
    canAttack = true;
    itsCoolDownTimer->stop();
}

void KnightModel::resetIframe()
{
    IframeTimer->stop();
    isInIframe = false;
}


