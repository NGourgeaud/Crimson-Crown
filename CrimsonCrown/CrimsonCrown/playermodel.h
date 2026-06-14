// File Name: playermodel.h
// Group: A11
// Description: Header file defining the base model for the player character, handling movement and state.

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
    ///
    /// \brief Initializes the player model with base statistics.
    /// \param aHp Starting health points.
    /// \param aMaxHP Maximum health points.
    /// \param aSpeed Movement speed.
    /// \param aAttack1Damage Damage for the primary attack.
    /// \param aAttack2Damage Damage for the secondary attack.
    /// \param aName The hero type selected.
    ///
    PlayerModel(int aHp, int aMaxHP, int aSpeed, int aAttack1Damage, int aAttack2Damage, HeroType aName);

    ///
    /// \brief Pure virtual function for the primary attack.
    ///
    virtual void attack1() = 0;

    ///
    /// \brief Pure virtual function for the secondary attack.
    ///
    virtual void attack2() = 0;

    ///
    /// \brief Updates the player's state per frame.
    ///
    void update();

    ///
    /// \brief Enables or disables leftward movement.
    /// \param enabled Movement state.
    ///
    void setMovementLeft(bool enabled);

    ///
    /// \brief Enables or disables rightward movement.
    /// \param enabled Movement state.
    ///
    void setMovementRight(bool enabled);

    ///
    /// \brief Enables or disables upward movement.
    /// \param enabled Movement state.
    ///
    void setMovementUp(bool enabled);

    ///
    /// \brief Enables or disables downward movement.
    /// \param enabled Movement state.
    ///
    void setMovementDown(bool enabled);

    ///
    /// \brief Sets the state of the primary attack.
    /// \param enabled True to trigger the attack.
    ///
    void setAttack1(bool enabled);

    ///
    /// \brief Sets the state of the secondary attack.
    /// \param enabled True to trigger the attack.
    ///
    void setAttack2(bool enabled);

    ///
    /// \brief Deducts health and activates invincibility frames.
    /// \param amount The amount of damage taken.
    ///
    void takeDamage(int amount);

    ///
    /// \brief Moves the player's hitbox to a specific top-left coordinate.
    /// \param pos The target coordinate.
    ///
    void move(QPointF pos);

    ///
    /// \brief Retrieves the geometric bounding box.
    /// \return A QRectF representing the physical space.
    ///
    QRectF getHitBox() const;

    ///
    /// \brief Sets the center of the player's hitbox.
    /// \param centre The target center point.
    ///
    void setCenter(QPoint centre);

    // Getters for the camera
    /// \brief Returns whether the player is moving up.
    /// \return True if moving up.
    bool getIsMovingUp() const { return isMovingUp; }

    /// \brief Returns whether the player is moving down.
    /// \return True if moving down.
    bool getIsMovingDown() const { return isMovingDown; }

    /// \brief Returns whether the player is moving left.
    /// \return True if moving left.
    bool getIsMovingLeft() const { return isMovingLeft; }

    /// \brief Returns whether the player is moving right.
    /// \return True if moving right.
    bool getIsMovingRight() const { return isMovingRight; }

    // Getters for health
    /// \brief Retrieves the current health points.
    /// \return The current HP.
    int getHP();

    /// \brief Retrieves the maximum health points.
    /// \return The maximum HP.
    int getMaxHP();

    ///
    /// \brief Restores the player's health to maximum.
    ///
    void regen();

    /// \brief Checks if the primary attack is ready.
    /// \return True if ready.
    bool getCanAttack1() const;

    /// \brief Checks if the secondary attack is ready.
    /// \return True if ready.
    bool getCanAttack2() const;

    /// \brief Retrieves remaining cooldown for the primary attack.
    /// \return Time in milliseconds.
    int getCooldown1Remaining() const;

    /// \brief Retrieves remaining cooldown for the secondary attack.
    /// \return Time in milliseconds.
    int getCooldown2Remaining() const;

    /// \brief Retrieves the specific hero type.
    /// \return The HeroType enumerator.
    HeroType getItsName() const;

    /// \brief Checks if the player holds the secret key.
    /// \return True if holding the key.
    bool getHasSecretKey() const { return hasSecretKey; }

    /// \brief Updates the player's secret key possession state.
    /// \param hasKey The new state.
    void setHasSecretKey(bool hasKey) { hasSecretKey = hasKey; }

    ///
    /// \brief Restores a specific amount of health points.
    /// \param amount The amount to heal.
    ///
    void heal(int amount);

    ///
    /// \brief Increases the player's damage by a percentage.
    /// \param percentage The damage increase multiplier (e.g., 0.1 for 10%).
    ///
    void increaseDamage(double percentage);

    ///
    /// \brief Increases the player's maximum health.
    /// \param amount The amount of HP to add.
    ///
    void increaseMaxHP(int amount);

    ///
    /// \brief Increases the player's movement speed.
    /// \param amount The speed increment.
    ///
    void increaseSpeed(int amount);

signals:
    /// \brief Emitted when the player dies.
    void death();

private slots:
    /// \brief Resets the primary attack cooldown.
    void resetAttackCooldown1();

    /// \brief Resets the secondary attack cooldown.
    void resetAttackCooldown2();

    /// \brief Disables the invincibility state.
    void resetIframe(); };

#endif // PLAYERMODEL_H
