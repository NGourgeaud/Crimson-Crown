// File Name: knightmodel.cpp
// Group: A11
// Description: Model class for the Knight enemy, handling health, AI pursuit logic, and directional sword attacks.

#include "knightmodel.h"
#include <QLineF>
#include <QTransform>

///
/// \brief Handles the Knight's AI logic, making it move towards the player if out of range.
/// \param heroPos The current position of the player's hero.
/// \return True if the Knight is in range to attack, false if it is moving.
///
bool KnightModel::handleAI(QPointF heroPos)
{
    // Goes towards the player if it's too far
    if (QLineF(heroPos, itsHitBox.center()).length() > KnightAttackHitboxWidth - 10){
        for (int i = 0; i < itsSpeed; i++){
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

///
/// \brief Initializes the Knight enemy with its specific base attributes.
/// \param maxHp The maximum and starting health points of the Knight.
/// \param speed The movement speed of the Knight.
/// \param x The starting X coordinate.
/// \param y The starting Y coordinate.
/// \param attackCooldown The base time in seconds between each attack.
/// \param attackDamage The amount of damage the Knight's attack deals.
///
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

///
/// \brief Updates the Knight's logic per frame, handling pursuit and initiating attacks.
/// \param heroPos The current position of the player's hero.
///
void KnightModel::update(QPointF heroPos)
{
    if(handleAI(heroPos) && canAttack)
        attack(heroPos);
}

///
/// \brief Executes the Knight's melee attack, creating a rotated rectangular sword hitbox aimed at the player.
/// \param heroPos The target position to angle the attack towards.
///
void KnightModel::attack(QPointF heroPos)
{
    // 1. Find the angle between the Knight and the hero
    QLineF aimLine(itsHitBox.center(), heroPos);
    qreal angle = aimLine.angle(); // Returns the angle in degrees (0 is right, 90 is up)

    // 2. Create the attack rectangle centered on 0,0
    QRect baseAttackRect(0, -KnightAttackHitboxHeight / 2, KnightAttackHitboxWidth, KnightAttackHitboxHeight);

    // 3. Initialize the QTransform
    QTransform transform;

    // 4. Translate the center of the coordinate system towards the character
    transform.translate(itsHitBox.center().x(), itsHitBox.center().y());

    // 5. Apply rotation
    transform.rotate(-angle);

    // 6. Generate the final polygon
    QPolygon attackHitbox = transform.mapToPolygon(baseAttackRect);
    emit attacking(HitZoneModel(attackHitbox, SWORD), itsAttackDamage);

    canAttack = false;
    itsCoolDownTimer->start(itsAttackCooldown * 1000);
}

///
/// \brief Deducts health from the Knight and activates invincibility frames.
/// \param amount The amount of damage points to deduct.
///
void KnightModel::takeDamage(int amount)
{
    if (!isInIframe){
        itsHP -= amount;
        IframeTimer->start(LancerIframe * 1000); // Note: Uses LancerIframe constant
    }
    if (itsHP <= 0)
        emit death(this);
}

///
/// \brief Forces the Knight's hitbox to move to a specific position.
/// \param pos The new coordinate to move the Knight to.
///
void KnightModel::move(QPointF pos)
{
    itsHitBox.moveTo(pos.toPoint());
}

///
/// \brief Retrieves the current geometric bounding box of the Knight.
/// \return A QRectF representing the Knight's physical space.
///
QRectF KnightModel::getHitBox() const
{
    return itsHitBox;
}

///
/// \brief Allows the Knight to attack again once the cooldown timer finishes.
///
void KnightModel::resetAttackCooldown()
{
    canAttack = true;
    itsCoolDownTimer->stop();
}

///
/// \brief Disables the invincibility state once the i-frame timer finishes.
///
void KnightModel::resetIframe()
{
    IframeTimer->stop();
    isInIframe = false;
}
