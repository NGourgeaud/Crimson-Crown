// File Name: knightmodel.h
// Group: A11
// Description: Header file defining the Knight enemy model and its capabilities.

#ifndef KNIGHTMODEL_H
#define KNIGHTMODEL_H

#include <QObject>
#include "enemymodel.h"
#include <QPolygon>
#include "hitzonemodel.h"

class KnightModel : public QObject, public EnemyModel
{
    Q_OBJECT
    friend class TestEntity;

private:
    QTimer * itsCoolDownTimer;
    bool canAttack;

    ///
    /// \brief Handles the Knight's AI logic.
    /// \param heroPos The current position of the player's hero.
    /// \return True if the Knight is in range to attack, false otherwise.
    ///
    bool handleAI(QPointF heroPos);

public:
    ///
    /// \brief Initializes the Knight enemy.
    /// \param maxHp The maximum and starting health points.
    /// \param speed The movement speed.
    /// \param x The starting X coordinate.
    /// \param y The starting Y coordinate.
    /// \param attackCooldown The base time between attacks.
    /// \param attackDamage The amount of damage dealt.
    ///
    KnightModel(int maxHp, int speed, int x, int y, float attackCooldown, int attackDamage);

    ///
    /// \brief Updates the Knight's logic per frame.
    /// \param heroPos The current position of the player's hero.
    ///
    void update(QPointF heroPos);

    ///
    /// \brief Executes the Knight's melee attack.
    /// \param heroPos The target position to aim at.
    ///
    void attack(QPointF heroPos);

    ///
    /// \brief Deducts health and activates invincibility frames.
    /// \param amount The amount of damage to deduct.
    ///
    void takeDamage(int amount);

    ///
    /// \brief Forces the Knight to move to a specific position.
    /// \param pos The new coordinate.
    ///
    void move(QPointF pos);

    ///
    /// \brief Retrieves the current geometric bounding box.
    /// \return A QRectF representing the physical space.
    ///
    QRectF getHitBox() const;

signals:
    ///
    /// \brief Emitted when the Knight's health drops to 0.
    /// \param model Pointer to this instance.
    ///
    void death(KnightModel * model);

    ///
    /// \brief Emitted when the Knight generates a melee attack.
    /// \param hitbox The geometric area of the attack.
    /// \param damage The damage to deal.
    ///
    void attacking(HitZoneModel hitbox, int damage);

private slots:
    ///
    /// \brief Allows the Knight to attack again.
    ///
    void resetAttackCooldown();

    ///
    /// \brief Disables the invincibility state.
    ///
    void resetIframe();
};

#endif // KNIGHTMODEL_H
