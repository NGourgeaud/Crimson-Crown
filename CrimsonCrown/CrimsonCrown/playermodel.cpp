// File Name: playermodel.cpp
// Group: A11
// Description: Base model class for the player character, handling movement, stats, health, and attack cooldowns.

#include "playermodel.h"

///
/// \brief Retrieves the hero type of the player.
/// \return The HeroType representing the specific character chosen.
///
HeroType PlayerModel::getItsName() const
{
    return itsName;
}

///
/// \brief Initializes the player model with base statistics and default state.
/// \param aHp Starting health points.
/// \param aMaxHP Maximum health points.
/// \param aSpeed Movement speed.
/// \param aAttack1Damage Damage for the primary attack.
/// \param aAttack2Damage Damage for the secondary attack.
/// \param aName The hero type selected.
///
PlayerModel::PlayerModel(int aHp, int aMaxHP, int aSpeed, int aAttack1Damage, int aAttack2Damage, HeroType aName)
    :itsDirection(Right), itsHitBox(QRectF(0, 0, playerWidth, playerHeight))
{
    isMovingUp = false;
    isMovingDown = false;
    isMovingLeft = false;
    isMovingRight = false;
    isAttack1 = false;
    isAttack2 = false;
    itsCooldown1 = new QTimer();
    itsCooldown2 = new QTimer();
    canAttack1 = true;
    canAttack2 = true;
    IframeTimer = new QTimer();
    isInIframe = false;

    itsHP = aHp;
    itsMaxHP = aMaxHP;
    itsSpeed = aSpeed;
    itsAttack1Damage = aAttack1Damage;
    itsAttack2Damage = aAttack2Damage;
    itsName = aName;

    connect(itsCooldown1, &QTimer::timeout, this, &PlayerModel::resetAttackCooldown1);
    connect(itsCooldown2, &QTimer::timeout, this, &PlayerModel::resetAttackCooldown2);
    connect(IframeTimer, &QTimer::timeout, this, &PlayerModel::resetIframe);
}

///
/// \brief Updates the player's position based on active movement states and handles attack triggers.
///
void PlayerModel::update()
{
    if(isMovingLeft && isMovingDown){
        itsHitBox.translate(-itsSpeed/2, itsSpeed/2);
        itsDirection = LeftDown;
    }
    else if(isMovingLeft && isMovingUp){
        itsHitBox.translate(-itsSpeed/2, -itsSpeed/2);
        itsDirection = LeftUp;
    }
    else if(isMovingRight && isMovingDown){
        itsHitBox.translate(itsSpeed/2, itsSpeed/2);
        itsDirection = RightDown;
    }
    else if(isMovingRight && isMovingUp){
        itsHitBox.translate(itsSpeed/2, -itsSpeed/2);
        itsDirection = RightUp;
    }
    else if(isMovingLeft){
        itsHitBox.translate(-itsSpeed, 0);
        itsDirection = Left;
    }
    else if(isMovingRight) {
        itsHitBox.translate(itsSpeed, 0);
        itsDirection = Right;
    }
    else if(isMovingDown){
        itsHitBox.translate(0, itsSpeed);
        itsDirection = Down;
    }
    else if(isMovingUp) {
        itsHitBox.translate(0, -itsSpeed);
        itsDirection = Up;
    }

    if(isAttack1){
        attack1();
    }
    else if(isAttack2){
        attack2();
    }
}

///
/// \brief Enables or disables leftward movement.
/// \param enabled True to start moving left, false to stop.
///
void PlayerModel::setMovementLeft(bool enabled)
{
    isMovingLeft = enabled;
}

///
/// \brief Enables or disables upward movement.
/// \param enabled True to start moving up, false to stop.
///
void PlayerModel::setMovementUp(bool enabled)
{
    isMovingUp = enabled;
}

///
/// \brief Enables or disables rightward movement.
/// \param enabled True to start moving right, false to stop.
///
void PlayerModel::setMovementRight(bool enabled)
{
    isMovingRight = enabled;
}

///
/// \brief Enables or disables downward movement.
/// \param enabled True to start moving down, false to stop.
///
void PlayerModel::setMovementDown(bool enabled)
{
    isMovingDown = enabled;
}

///
/// \brief Sets the state of the primary attack.
/// \param enabled True to trigger the primary attack.
///
void PlayerModel::setAttack1(bool enabled)
{
    isAttack1 = enabled;
}

///
/// \brief Sets the state of the secondary attack.
/// \param enabled True to trigger the secondary attack.
///
void PlayerModel::setAttack2(bool enabled)
{
    isAttack2 = enabled;
}

///
/// \brief Deducts health from the player and activates invincibility frames.
/// \param amount The amount of damage points to deduct.
/// \details Fires the death signal if HP drops to or below 0.
///
void PlayerModel::takeDamage(int amount)
{
    if (!isInIframe){
        itsHP -= amount;
        isInIframe = true;
        IframeTimer->start(playerIframe * 1000);
    }
    if (itsHP <= 0)
        emit death();
}

///
/// \brief Moves the player's hitbox to a specific top-left coordinate.
/// \param pos The new target coordinate.
///
void PlayerModel::move(QPointF pos)
{
    itsHitBox.moveTopLeft(pos);
}

///
/// \brief Resets the primary attack cooldown timer.
///
void PlayerModel::resetAttackCooldown1()
{
    canAttack1 = true;
    itsCooldown1->stop();
}

///
/// \brief Resets the secondary attack cooldown timer.
///
void PlayerModel::resetAttackCooldown2()
{
    canAttack2 = true;
    itsCooldown2->stop();
}

///
/// \brief Disables the invincibility state once the i-frame timer finishes.
///
void PlayerModel::resetIframe()
{
    isInIframe = false;
    IframeTimer->stop();
}

///
/// \brief Retrieves the current geometric bounding box of the player.
/// \return A QRectF representing the player's physical space.
///
QRectF PlayerModel::getHitBox() const
{
    return itsHitBox;
}

///
/// \brief Moves the player's hitbox to center on a specific coordinate.
/// \param centre The new center coordinate.
///
void PlayerModel::setCenter(QPoint centre)
{
    itsHitBox.moveCenter(centre);
}

///
/// \brief Retrieves the player's current health points.
/// \return An integer representing the current HP.
///
int PlayerModel::getHP()
{
    return itsHP;
}

///
/// \brief Retrieves the player's maximum health points.
/// \return An integer representing the maximum HP.
///
int PlayerModel::getMaxHP()
{
    return itsMaxHP;
}

///
/// \brief Fully restores the player's health to maximum.
///
void PlayerModel::regen()
{
    itsHP = itsMaxHP;
}

///
/// \brief Checks if the primary attack is ready to be used.
/// \return True if the attack is off cooldown, false otherwise.
///
bool PlayerModel::getCanAttack1() const
{
    return canAttack1;
}

///
/// \brief Checks if the secondary attack is ready to be used.
/// \return True if the attack is off cooldown, false otherwise.
///
bool PlayerModel::getCanAttack2() const
{
    return canAttack2;
}

///
/// \brief Retrieves the remaining time for the primary attack cooldown.
/// \return Time remaining in milliseconds, or 0 if ready.
///
int PlayerModel::getCooldown1Remaining() const
{
    return itsCooldown1->isActive() ? itsCooldown1->remainingTime() : 0;
}

///
/// \brief Retrieves the remaining time for the secondary attack cooldown.
/// \return Time remaining in milliseconds, or 0 if ready.
///
int PlayerModel::getCooldown2Remaining() const
{
    return itsCooldown2->isActive() ? itsCooldown2->remainingTime() : 0;
}

///
/// \brief Restores a specific amount of health points to the player.
/// \param amount The amount of HP to heal.
///
void PlayerModel::heal(int amount)
{
    itsHP += amount;
    if (itsHP > itsMaxHP) {
        itsHP = itsMaxHP; // Cap at maximum health points
    }
}

///
/// \brief Permanently increases the damage of both character attacks by a percentage.
/// \param percentage The percentage to increase (e.g., 0.10 for 10%).
///
void PlayerModel::increaseDamage(double percentage)
{
    // Increases the damage of both character attacks by the given percentage
    itsAttack1Damage += static_cast<int>(itsAttack1Damage * percentage);
    itsAttack2Damage += static_cast<int>(itsAttack2Damage * percentage);
}

///
/// \brief Permanently increases the maximum health points and heals the player by that amount.
/// \param amount The flat amount of max HP to add.
///
void PlayerModel::increaseMaxHP(int amount)
{
    itsMaxHP += amount;
    itsHP += amount;
}

///
/// \brief Permanently increases the player's movement speed.
/// \param amount The flat amount of speed to add.
///
void PlayerModel::increaseSpeed(int amount)
{
    itsSpeed += amount;
}
