// File Name: lancermodel.h
// Group: A11
// Description: Header file defining the Lancer enemy model and its capabilities.

#ifndef LANCERMODEL_H
#define LANCERMODEL_H

#include "enemymodel.h"
#include "hitzonemodel.h"
#include <QObject>
#include <QPolygon>
#include <QTimer>

class LancerModel : public QObject, public EnemyModel
{
    Q_OBJECT
    friend class TestEntity;

public:
    ///
    /// \brief Initializes the Lancer enemy.
    /// \param maxHp The maximum and starting health points.
    /// \param speed The movement speed.
    /// \param x The starting X coordinate.
    /// \param y The starting Y coordinate.
    /// \param attackCooldown The base time between attacks.
    /// \param attackDamage The amount of damage dealt.
    ///
    LancerModel(int maxHp, int speed, int x, int y, float attackCooldown, int attackDamage);

    ///
    /// \brief Updates the Lancer's logic per frame.
    /// \param heroPos The current position of the player's hero.
    ///
    void update(QPointF heroPos);

    ///
    /// \brief Executes the Lancer's melee attack.
    /// \param heroPos The target position to aim at.
    ///
    void attack(QPointF heroPos);

    ///
    /// \brief Deducts health and activates invincibility frames.
    /// \param amount The amount of damage to deduct.
    ///
    void takeDamage(int amount);

    ///
    /// \brief Forces the Lancer to move to a specific position.
    /// \param pos The new coordinate.
    ///
    void move(QPointF pos);

    ///
    /// \brief Retrieves the current geometric bounding box.
    /// \return A QRectF representing the physical space.
    ///
    QRectF getHitBox() const;

private:
    QTimer * itsCoolDownTimer;
    bool canAttack;

    ///
    /// \brief Handles the Lancer's AI logic.
    /// \param heroPos The current position of the player's hero.
    /// \return True if the Lancer is in range to attack, false otherwise.
    ///
    bool handleAI(QPointF heroPos);

signals:
    ///
    /// \brief Emitted when the Lancer's health drops to 0.
    /// \param model Pointer to this instance.
    ///
    void death(LancerModel * model);

    ///
    /// \brief Emitted when the Lancer generates an attack.
    /// \param hitbox The geometric area of the attack.
    /// \param damage The damage to deal.
    ///
    void attacking(HitZoneModel hitbox, int damage);

private slots:
    ///
    /// \brief Allows the Lancer to attack again.
    ///
    void resetAttackCooldown();

    ///
    /// \brief Disables the invincibility state.
    ///
    void resetIframe();
};

#endif // LANCERMODEL_H
