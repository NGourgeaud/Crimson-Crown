// File Name: clergymodel.h
// Group: A11
// Description: Header file defining the Clergy boss/enemy model and its capabilities.

#ifndef CLERGYMODEL_H
#define CLERGYMODEL_H

#include "bossmodel.h"
#include "projectilemodel.h"
#include <QtCore/qline.h>
#include <QtCore/qtimer.h>

class clergyModel: public QObject, public BossModel
{
    Q_OBJECT
public:
    ///
    /// \brief Initializes the Clergy enemy.
    /// \param maxHp The maximum and starting health points.
    /// \param speed Movement speed.
    /// \param x Starting X coordinate.
    /// \param y Starting Y coordinate.
    /// \param attack1Cooldown Base cooldown for the primary attack.
    /// \param attack2Cooldown Base cooldown for the secondary attack.
    /// \param attack1Damage Damage dealt by the primary attack.
    /// \param attack2Damage Damage dealt by the secondary attack.
    ///
    clergyModel(int maxHp, int speed, int x, int y, float attack1Cooldown,float attack2Cooldown, int attack1Damage, int attack2Damage);

    ///
    /// \brief Updates the logic per frame.
    /// \param heroPos The position of the player's hero.
    ///
    void update(QPointF heroPos);

    ///
    /// \brief Deducts health and activates invincibility frames.
    /// \param amount The damage points to deduct.
    ///
    void takeDamage(int amount);

    ///
    /// \brief Moves the hitbox to a specific position.
    /// \param pos The new coordinate.
    ///
    void move(QPointF pos);

    ///
    /// \brief Retrieves the geometric bounding box.
    /// \return A QRectF representing the physical space.
    ///
    QRectF getHitBox() const;

    ///
    /// \brief Handles the evasion AI.
    /// \param heroPos The position of the player's hero.
    /// \return True if holding position, false if fleeing.
    ///
    bool handleAI(QPointF heroPos);

    ///
    /// \brief Executes the primary single-projectile attack.
    /// \param heroPos Target position.
    ///
    void attack1(QPointF heroPos);

    ///
    /// \brief Executes the secondary multi-projectile attack.
    /// \param heroPos Target position to set the base angle.
    ///
    void attack2(QPointF heroPos);

private:
    QTimer * itsCoolDownTimer1;
    QTimer * itsCoolDownTimer2;
    bool canAttack1;
    bool canAttack2;
    QLineF currentAimLine;

signals:
    ///
    /// \brief Emitted upon health depletion.
    /// \param Boss Pointer to this instance.
    ///
    void death(BossModel * Boss);

    ///
    /// \brief Emitted to spawn a projectile.
    /// \param hitbox Pointer to the projectile model.
    ///
    void attacking(ProjectileModel * hitbox);

private slots:
    ///
    /// \brief Resets primary attack cooldown.
    ///
    void resetAttack1Cooldown();

    ///
    /// \brief Resets secondary attack cooldown.
    ///
    void resetAttack2Cooldown();

    ///
    /// \brief Disables invincibility.
    ///
    void resetIframe();
};

#endif // CLERGYMODEL_H
