// File Name: archermodel.h
// Group: A11
// Description: Header file defining the Archer enemy model and its capabilities.

#ifndef ARCHERMODEL_H
#define ARCHERMODEL_H

#include <QObject>
#include "enemymodel.h"
#include "projectilemodel.h"

class ArcherModel : public QObject, public EnemyModel
{
    Q_OBJECT

public:
    ///
    /// \brief Initializes an Archer enemy with health, speed, and randomized attack timing.
    /// \param maxHp The maximum and starting health points.
    /// \param speed The movement speed.
    /// \param x The starting X coordinate.
    /// \param y The starting Y coordinate.
    /// \param attackCooldown The base time in seconds between attacks.
    /// \param attackDamage The amount of damage dealt by projectiles.
    ///
    ArcherModel(int maxHp, int speed, int x, int y, float attackCooldown, int attackDamage);

    ///
    /// \brief Updates the archer's logic per frame.
    /// \param heroPos The current position of the player's hero.
    ///
    void update(QPointF heroPos);

    ///
    /// \brief Fires an arrow projectile towards the player's position.
    /// \param heroPos The target position to aim at.
    ///
    void attack(QPointF heroPos);

    ///
    /// \brief Deducts health and activates invincibility frames.
    /// \param amount The amount of damage to deduct.
    ///
    void takeDamage(int amount);

    ///
    /// \brief Forces the archer to move to a specific position.
    /// \param pos The new coordinate.
    ///
    void move(QPointF pos);

    ///
    /// \brief Retrieves the current geometric bounding box.
    /// \return A QRectF representing the physical space.
    ///
    QRectF getHitBox() const;

    ///
    /// \brief Handles the archer's artificial intelligence.
    /// \param heroPos The current position of the player's hero.
    /// \return True if ready to attack, false otherwise.
    ///
    bool handleAI(QPointF heroPos);

private:
    QTimer * itsCoolDownTimer;
    bool canAttack;

signals:
    ///
    /// \brief Emitted when the archer's health drops to 0.
    /// \param model Pointer to this instance.
    ///
    void death(ArcherModel * model);

    ///
    /// \brief Emitted when the archer fires a projectile.
    /// \param projectile Pointer to the created projectile.
    ///
    void attacking(ProjectileModel * projectile);

private slots:
    ///
    /// \brief Allows the archer to attack again.
    ///
    void resetAttackCooldown();

    ///
    /// \brief Disables the invincibility state.
    ///
    void resetIframe();
};

#endif // ARCHERMODEL_H
