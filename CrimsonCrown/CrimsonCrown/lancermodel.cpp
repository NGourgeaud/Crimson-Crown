// File Name: lancermodel.cpp
// Group: A11
// Description: Model class for the Lancer enemy, managing health, AI tracking, and directional melee attacks.

#include "lancermodel.h"
#include "config.h"
#include "qline.h"
#include "qtransform.h"

///
/// \brief Initializes the Lancer enemy with specific base attributes.
/// \param maxHp The maximum and starting health points of the Lancer.
/// \param speed The movement speed of the Lancer.
/// \param x The starting X coordinate.
/// \param y The starting Y coordinate.
/// \param attackCooldown The base time in seconds between each attack.
/// \param attackDamage The amount of damage the Lancer deals.
///
LancerModel::LancerModel(int maxHp, int speed, int x, int y, float attackCooldown, int attackDamage) : EnemyModel()
{
    itsHP = maxHp;
    itsMaxHP = maxHp;
    itsSpeed = speed;
    itsHitBox = QRectF(x, y, LancerWidth, LancerHeight);
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
/// \brief Updates the Lancer's logic per frame, handling pursuit and initiating attacks.
/// \param heroPos The current position of the player's hero.
///
void LancerModel::update(QPointF heroPos)
{
    if(handleAI(heroPos) && canAttack)
        attack(heroPos);
}

///
/// \brief Executes the Lancer's melee attack, generating a hit zone and sending it to the GameModel.
/// \param heroPos The current position of the player's hero to aim the attack.
///
void LancerModel::attack(QPointF heroPos)
{
    // 1. Find the angle between the lancer and the hero
    QLineF aimLine(itsHitBox.center(), heroPos);
    qreal angle = aimLine.angle(); // Returns the angle in degrees (0 is right, 90 is up)

    // 2. Create the attack rectangle centered on 0,0
    QRect baseAttackRect(0, -LancerAttackHitboxHeight / 2, LancerAttackHitboxWidth, LancerAttackHitboxHeight);

    // 3. Initialize the QTransform
    QTransform transform;

    // 4. Translate the center of the coordinate system towards your character
    transform.translate(itsHitBox.center().x(), itsHitBox.center().y());

    // 5. Apply rotation
    transform.rotate(-angle);

    // 6. Generate the final polygon
    QPolygon attackHitbox = transform.mapToPolygon(baseAttackRect);
    emit attacking(HitZoneModel(attackHitbox, LANCE), itsAttackDamage);

    canAttack = false;
    itsCoolDownTimer->start(itsAttackCooldown * 1000);
}

///
/// \brief Handles the AI logic, making the Lancer move towards the player if out of range.
/// \param heroPos The current position of the player's hero.
/// \return True if the Lancer is in range and can attack, false if it is moving.
///
bool LancerModel::handleAI(QPointF heroPos)
{
    // Moves towards the player if they are too far
    for (int i = 0; i < itsSpeed; i++){
        if (QLineF(heroPos, itsHitBox.center()).length() > LancerAttackHitboxWidth - 5){

            QLineF aimLine = QLineF(itsHitBox.center(), heroPos);
            qreal angle = aimLine.angle();
            QLineF movementVector = QLineF::fromPolar(1, angle);
            itsHitBox.translate(movementVector.dx(), movementVector.dy());

            if (i == itsSpeed - 1)
                return false;
        }
    }
    return true;
}

///
/// \brief Resets the attack cooldown, allowing the Lancer to strike again.
///
void LancerModel::resetAttackCooldown()
{
    canAttack = true;
    itsCoolDownTimer->stop();
}

///
/// \brief Disables the invincibility state once the i-frame timer finishes.
///
void LancerModel::resetIframe()
{
    IframeTimer->stop();
    isInIframe = false;
}

///
/// \brief Deducts health from the Lancer and activates invincibility frames.
/// \param amount The amount of damage points to deduct.
/// \details Fires the death signal if HP drops to or below 0.
///
void LancerModel::takeDamage(int amount)
{
    if (!isInIframe){
        itsHP -= amount;
        IframeTimer->start(LancerIframe * 1000);
    }
    if (itsHP <= 0)
        emit death(this);
}

///
/// \brief Moves the Lancer's hitbox to a specific top-left coordinate.
/// \param pos The new target coordinate.
///
void LancerModel::move(QPointF pos)
{
    itsHitBox.moveTo(pos.toPoint());
}

///
/// \brief Retrieves the current geometric bounding box of the Lancer.
/// \return A QRectF representing the Lancer's physical space.
///
QRectF LancerModel::getHitBox() const
{
    return itsHitBox;
}
