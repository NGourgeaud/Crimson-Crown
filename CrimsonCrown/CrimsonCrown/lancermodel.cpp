#include "lancermodel.h"
#include "config.h"
#include "qline.h"
#include "qtransform.h"



LancerModel::LancerModel(int maxHp, int speed, int x, int y, float attackCooldown, int attackDamage) : EnemyModel()
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
    itsType = LANCER;

    connect(itsCoolDownTimer, &QTimer::timeout, this, &LancerModel::resetAttackCooldown);
    connect(IframeTimer, &QTimer::timeout, this, &LancerModel::resetIframe);
    itsCoolDownTimer->start(1500);
}

///
/// \brief Updates the Lancer
/// \param heroX: x position of the center
/// \param heroY: y position of the center
///
void LancerModel::update(QPointF heroPos)
{
    if(handleAI(heroPos) and canAttack)
        attack(heroPos);
}

///
/// \brief Makes the Lancer attack the ennemy (sends a hitbox to GameModel to check for hits)
/// \param heroX: x position of the center
/// \param heroY: y position of the center
///
void LancerModel::attack(QPointF heroPos)
{
    // 1. Find the angle between the lancer and the hero
    QLineF aimLine(itsHitBox.center(), heroPos);
    qreal angle = aimLine.angle(); // Returns the angle in degrees (0 is right, 90 is up)

    // 1. Create the attack rectangle centered on 0,0
    QRect baseAttackRect(0, -LancerAttackHitboxHeight / 2, LancerAttackHitboxWidth, LancerAttackHitboxHeight);

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

///
/// \brief Takes the hero position and processes the AI logic depending on it's position, also returns true if the ennemy can attack
/// \param heroX: x position of the center
/// \param heroY: y position of the center
///
bool LancerModel::handleAI(QPointF heroPos)
{
    // Goes towards the player if it's too far
    for (int i = 0; i<itsSpeed; i++){
        if (QLineF(heroPos, itsHitBox.center()).length() > LancerAttackHitboxWidth-5){

            QLineF aimLine = QLineF(itsHitBox.center(), heroPos);
            qreal angle = aimLine.angle();
            QLineF movementVector = QLineF::fromPolar(1, angle);
            itsHitBox.translate(movementVector.dx(), movementVector.dy());

            if (i == itsSpeed-1)
                return false;
        }
    }
    return true;
}

///
/// \brief Called to reset the atack cooldown
///
void LancerModel::resetAttackCooldown()
{
    canAttack = true;
    itsCoolDownTimer->stop();
}

void LancerModel::resetIframe()
{
    IframeTimer->stop();
    isInIframe = false;
}

///
/// \brief makes the Lancer take damage
/// \param amount: Amount of damage taken
/// \details if hp<=0 fires death signal
///
void LancerModel::takeDamage(int amount)
{
    if (!isInIframe){
        itsHP-= amount;
        IframeTimer->start(LancerIframe*1000);
    }
    if (itsHP<=0)
        emit death(this);
}

///
/// \brief Moves the player at the set position (top left)
/// \param axisX: x center
/// \param axisY: y center
///
void LancerModel::move(QPointF pos)
{
    itsHitBox.moveTo(pos.toPoint());
}

///
/// \brief Gets the Lancer's hitbox
/// \return Lancer's QRect
///
QRectF LancerModel::getHitBox() const
{
    return itsHitBox;
}
