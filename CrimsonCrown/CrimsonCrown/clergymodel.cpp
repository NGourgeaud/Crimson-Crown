// File Name: clergymodel.cpp
// Group: A11
// Description: Model class for the Clergy enemy, managing its health, evasion AI, and multi-directional projectile attacks.

#include "clergymodel.h"
#include "config.h"
#include "projectilemodel.h"
#include <QPainterpath>

///
/// \brief Initializes the Clergy enemy with health, speed, and dual attack cooldowns.
/// \param maxHp The maximum and starting health points of the Clergy.
/// \param speed The movement speed of the Clergy.
/// \param x The starting X coordinate.
/// \param y The starting Y coordinate.
/// \param attack1Cooldown The base time in seconds between each primary attack.
/// \param attack2Cooldown The base time in seconds between each secondary area attack.
/// \param attack1Damage The damage dealt by the primary attack.
/// \param attack2Damage The damage dealt by the secondary attack.
///
clergyModel::clergyModel(int maxHp, int speed, int x, int y, float attack1Cooldown, float attack2Cooldown, int attack1Damage, int attack2Damage)
{
    itsHP = maxHp;
    itsMaxHP = maxHp;
    itsSpeed = speed;
    itsHitBox = QRectF(x,y, ClergyWidth, ClergyHeight);
    itsAttack1Cooldown = attack1Cooldown;
    itsAttack2Cooldown = attack2Cooldown;
    itsAttack1Damage = attack1Damage;
    itsAttack2Damage = attack2Damage;
    itsCoolDownTimer1 = new QTimer();
    itsCoolDownTimer2 = new QTimer();
    canAttack1 = false;
    canAttack2 = false;
    IframeTimer = new QTimer();
    isInIframe = false;
    itsType = CLERGY;

    connect(itsCoolDownTimer1, &QTimer::timeout, this, &clergyModel::resetAttack1Cooldown);
    connect(itsCoolDownTimer2, &QTimer::timeout, this, &clergyModel::resetAttack2Cooldown);
    connect(IframeTimer, &QTimer::timeout, this, &clergyModel::resetIframe);

    // Initial delays before the first attacks can be launched
    itsCoolDownTimer1->start(1500);
    itsCoolDownTimer2->start(1500);
}

///
/// \brief Updates the Clergy's logic per frame, handling movement and both attacks.
/// \param heroPos The current position of the player's hero.
///
void clergyModel::update(QPointF heroPos)
{
    // 1. Handle AI movement (evasion)
    handleAI(heroPos);

    // 2. Handle primary attack (single projectile)
    if(canAttack1) {
        attack1(heroPos);
    }

    // 3. Handle secondary attack (multi-directional projectiles)
    if (canAttack2) {
        attack2(heroPos);
    }
}

///
/// \brief Executes the Clergy's primary attack, firing a single projectile at the player.
/// \param heroPos The target position to aim the projectile at.
///
void clergyModel::attack1(QPointF heroPos)
{
    QLineF aimLine(itsHitBox.center(), heroPos);

    QRectF projHitbox(0, 0, ClergyAttack1HitBoxWidth, ClergyAttack1HitBoxHeight);
    projHitbox.moveCenter(itsHitBox.center());
    ProjectileModel * projectile = new ProjectileModel(projHitbox, aimLine, 3, ClergyDefaultAttack1Damage, ENEMY, RADIUS);

    emit attacking(projectile);
    canAttack1 = false;
    itsCoolDownTimer1->start(ClergyDefaultAttack1CoolDown * 1000);
}

///
/// \brief Executes the Clergy's secondary attack, firing 6 projectiles in a hexagonal pattern.
/// \param heroPos The current position of the player to set the base angle.
///
void clergyModel::attack2(QPointF heroPos)
{
    QLineF aimLine1(itsHitBox.center(), heroPos);

    // Adds 60 degrees iteratively to create a star/hexagon firing pattern
    QLineF aimLine2 = aimLine1;
    aimLine2.setAngle(aimLine1.angle() + 60);

    QLineF aimLine3 = aimLine2;
    aimLine3.setAngle(aimLine2.angle() + 60);

    QLineF aimLine4 = aimLine3;
    aimLine4.setAngle(aimLine3.angle() + 60);

    QLineF aimLine5 = aimLine4;
    aimLine5.setAngle(aimLine4.angle() + 60);

    QLineF aimLine6 = aimLine5;
    aimLine6.setAngle(aimLine5.angle() + 60);

    QRectF projHitbox(0, 0, ClergyAttack1HitBoxWidth, ClergyAttack1HitBoxHeight);
    projHitbox.moveCenter(itsHitBox.center());

    ProjectileModel * projectile1 = new ProjectileModel(projHitbox, aimLine1, 3, ClergyDefaultAttack1Damage, ENEMY, RADIUS);
    ProjectileModel * projectile2 = new ProjectileModel(projHitbox, aimLine2, 3, ClergyDefaultAttack1Damage, ENEMY, RADIUS);
    ProjectileModel * projectile3 = new ProjectileModel(projHitbox, aimLine3, 3, ClergyDefaultAttack1Damage, ENEMY, RADIUS);
    ProjectileModel * projectile4 = new ProjectileModel(projHitbox, aimLine4, 3, ClergyDefaultAttack1Damage, ENEMY, RADIUS);
    ProjectileModel * projectile5 = new ProjectileModel(projHitbox, aimLine5, 3, ClergyDefaultAttack1Damage, ENEMY, RADIUS);
    ProjectileModel * projectile6 = new ProjectileModel(projHitbox, aimLine6, 3, ClergyDefaultAttack1Damage, ENEMY, RADIUS);

    emit attacking(projectile1);
    emit attacking(projectile2);
    emit attacking(projectile3);
    emit attacking(projectile4);
    emit attacking(projectile5);
    emit attacking(projectile6);

    canAttack2 = false;
    itsCoolDownTimer2->start(ClergyDefaultAttack2CoolDown * 1000);
}

///
/// \brief Deducts health from the Clergy and activates invincibility frames.
/// \param amount The amount of damage points to deduct from the Clergy's health.
///
void clergyModel::takeDamage(int amount)
{
    if (!isInIframe){
        itsHP -= amount;
        isInIframe = true;
        IframeTimer->start(ClergyIframe * 1000);
    }

    // Checks if the Clergy's health has depleted to trigger the death signal
    if (itsHP <= 0)
        emit death(this);
}

///
/// \brief Forces the Clergy's hitbox to move to a specific position.
/// \param pos The new coordinate to move the Clergy to.
///
void clergyModel::move(QPointF pos)
{
    itsHitBox.moveTo(pos);
}

///
/// \brief Retrieves the current geometric bounding box of the Clergy.
/// \return A QRectF representing the Clergy's physical space and position.
///
QRectF clergyModel::getHitBox() const
{
    return itsHitBox;
}

///
/// \brief Handles the Clergy's evasion AI, making it flee in the opposite direction if the player gets too close.
/// \param heroPos The current position of the player's hero.
/// \return True if the Clergy is maintaining its position, false if it is actively fleeing.
///
bool clergyModel::handleAI(QPointF heroPos)
{
    // If the player is too close (less than 200 pixels)
    if (QLineF(heroPos, itsHitBox.center()).length() < 200) {

        QLineF aimLine(itsHitBox.center(), heroPos);

        // Creates a movement vector:
        // Length = movement speed
        // Direction = Angle towards the player + 180 degrees (to move in the opposite direction)
        QLineF movementVector = QLineF::fromPolar(itsSpeed, aimLine.angle() + 180);

        // Translates the hitbox based on the calculated vector
        itsHitBox.translate(movementVector.dx(), movementVector.dy());

        return false;
    }

    return true;
}

///
/// \brief Allows the Clergy to use its primary attack again once the cooldown timer finishes.
///
void clergyModel::resetAttack1Cooldown()
{
    canAttack1 = true;
    itsCoolDownTimer1->stop();
}

///
/// \brief Allows the Clergy to use its secondary attack again once the cooldown timer finishes.
///
void clergyModel::resetAttack2Cooldown()
{
    canAttack2 = true;
    itsCoolDownTimer2->stop();
}

///
/// \brief Disables the invincibility state once the i-frame timer finishes.
///
void clergyModel::resetIframe()
{
    IframeTimer->stop();
    isInIframe = false;
}
