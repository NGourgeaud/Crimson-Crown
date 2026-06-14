// File Name: kingmodel.h
// Group: A11
// Description: Header file defining the King boss model and its abilities.

#ifndef KINGMODEL_H
#define KINGMODEL_H

#include <QObject>
#include "bossmodel.h"
#include "hitzonemodel.h"

class KingModel : public QObject, public BossModel
{
    Q_OBJECT

public:
    ///
    /// \brief Initializes the King boss with specific stats.
    /// \param maxHp The maximum and starting health points.
    /// \param speed The movement speed.
    /// \param x The starting X coordinate.
    /// \param y The starting Y coordinate.
    /// \param attack1Cooldown Base cooldown for the primary attack.
    /// \param attack2Cooldown Base cooldown for the secondary attack.
    /// \param attack1Damage Damage dealt by the primary attack.
    /// \param attack2Damage Damage dealt by the secondary attack.
    ///
    KingModel(int maxHp, int speed, int x, int y, float attack1Cooldown, float attack2Cooldown, int attack1Damage, int attack2Damage);

    ///
    /// \brief Updates the King's logic per frame.
    /// \param heroPos The current position of the player's hero.
    ///
    void update(QPointF heroPos);

    ///
    /// \brief Deducts health and activates invincibility frames.
    /// \param amount The amount of damage to deduct.
    ///
    void takeDamage(int amount);

    ///
    /// \brief Forces the King to move to a specific position.
    /// \param pos The new coordinate.
    ///
    void move(QPointF pos);

    ///
    /// \brief Retrieves the current geometric bounding box.
    /// \return A QRectF representing the physical space.
    ///
    QRectF getHitBox() const;

    ///
    /// \brief Sets whether the King can summon minions.
    /// \param newCanSummon Boolean indicating summoning capability.
    ///
    void setCanSummon(bool newCanSummon);

    ///
    /// \brief Handles the King's AI logic.
    /// \param heroPos The current position of the player's hero.
    /// \return True if in range, false if moving.
    ///
    bool handleAI(QPointF heroPos);

    ///
    /// \brief Executes the primary attack (summoning).
    /// \param heroPos The target position.
    ///
    void attack1(QPointF heroPos);

    ///
    /// \brief Initiates the secondary jump attack.
    /// \param heroPos The target position.
    ///
    void attack2(QPointF heroPos);

private:
    QTimer * itsCoolDownTimer1;
    QTimer * itsCoolDownTimer2;
    QTimer * itsSummoningTimer;
    QTimer * itsjumpingTimer;
    QTimer * itsAttack2Step2Timer;
    bool canAttack1;
    bool canAttack2;

    bool isSummoning;
    bool isJumping;
    bool canSummon;

signals:
    ///
    /// \brief Emitted when the King's health drops to 0.
    /// \param Boss Pointer to this instance.
    ///
    void death(BossModel * Boss);

    ///
    /// \brief Emitted to spawn enemy minions.
    ///
    void spawnEnemies();

    ///
    /// \brief Emitted when the King generates an attack hit zone.
    /// \param hitbox The geometric area of the attack.
    /// \param damage The damage to deal.
    ///
    void attacking(HitZoneModel hitbox, int damage);

private slots:
    ///
    /// \brief Completes the summoning phase.
    ///
    void finishSummoning();

    ///
    /// \brief Executes the first step of the jump attack (landing).
    ///
    void attack2Step1();

    ///
    /// \brief Executes the second step of the jump attack (shockwaves).
    ///
    void attack2Step2();

    ///
    /// \brief Resets primary attack cooldown.
    ///
    void resetAttack1Cooldown();

    ///
    /// \brief Resets secondary attack cooldown.
    ///
    void resetAttack2Cooldown();

    ///
    /// \brief Disables the invincibility state.
    ///
    void resetIframe();
};

#endif // KINGMODEL_H
