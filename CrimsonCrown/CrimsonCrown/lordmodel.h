// File Name: lordmodel.h
// Group: A11
// Description: Header file defining the Lord boss model and its capabilities.

#ifndef LORDMODEL_H
#define LORDMODEL_H

#include <QObject>
#include "bossmodel.h"
#include <QPolygon>
#include "hitzonemodel.h"
#include <QLineF>
#include <QTimer>

class LordModel : public QObject, public BossModel
{
    Q_OBJECT
public:
    ///
    /// \brief Initializes the Lord boss.
    /// \param maxHp Maximum health points.
    /// \param speed Movement speed.
    /// \param x Starting X coordinate.
    /// \param y Starting Y coordinate.
    /// \param attack1Cooldown Cooldown for the primary attack.
    /// \param attack2Cooldown Cooldown for the secondary dash attack.
    /// \param attack1Damage Damage for the primary attack.
    /// \param attack2Damage Damage for the secondary attack.
    ///
    LordModel(int maxHp, int speed, int x, int y, float attack1Cooldown, float attack2Cooldown, int attack1Damage, int attack2Damage);

    ///
    /// \brief Updates the Lord's logic per frame.
    /// \param heroPos The current position of the player's hero.
    ///
    void update(QPointF heroPos);

    ///
    /// \brief Deducts health and activates invincibility frames.
    /// \param amount The amount of damage to deduct.
    ///
    void takeDamage(int amount);

    ///
    /// \brief Forces the Lord to move to a specific position.
    /// \param pos The new coordinate.
    ///
    void move(QPointF pos);

    ///
    /// \brief Retrieves the current geometric bounding box.
    /// \return A QRectF representing the physical space.
    ///
    QRectF getHitBox() const;

    ///
    /// \brief Handles the Lord's AI logic.
    /// \param heroPos The current position of the player's hero.
    /// \return True if in range, false if moving.
    ///
    bool handleAI(QPointF heroPos);

    ///
    /// \brief Executes the primary spin attack.
    /// \param heroPos The target position.
    ///
    void attack1(QPointF heroPos);

    ///
    /// \brief Initiates the secondary dash attack.
    /// \param heroPos The target position.
    ///
    void attack2(QPointF heroPos);

private:
    QTimer * itsCoolDownTimer1;
    QTimer * itsCoolDownTimer2;
    bool canAttack1;
    bool canAttack2;

    int currentDashStep;
    int maxDashSteps;
    bool isDashing;
    QTimer * itsDashTimer;
    QLineF currentAimLine;

signals:
    ///
    /// \brief Emitted when the Lord's health drops to 0.
    /// \param Boss Pointer to this instance.
    ///
    void death(BossModel * Boss);

    ///
    /// \brief Emitted when the Lord generates an attack.
    /// \param hitbox The geometric area of the attack.
    /// \param damage The damage to deal.
    ///
    void attacking(HitZoneModel hitbox, int damage);

private slots:
    /// \brief Resets primary attack cooldown.
    void resetAttack1Cooldown();

    /// \brief Resets secondary attack cooldown.
    void resetAttack2Cooldown();

    /// \brief Disables the invincibility state.
    void resetIframe();

    /// \brief Processes each step of the active dash attack.
    void dashStep(); };

#endif // LORDMODEL_H
