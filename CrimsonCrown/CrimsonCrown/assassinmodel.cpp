// File Name: assassinmodel.cpp
// Group: A11
// Description: Model class for the Assassin hero, handling specific stats, knife projectile attacks, and smoke bomb invisibility mechanics.

#include "assassinmodel.h"
#include "config.h"

///
/// \brief Initializes the Assassin player character with its base stats and abilities.
///
AssassinModel::AssassinModel()
    : PlayerModel(18, 18, 12, 6, 0, HeroType::Assassin)
{
    smokeBombTimer = new QTimer(this);
    smokeBombTimer->setSingleShot(true);
    isInvisible = false;
    connect(smokeBombTimer, &QTimer::timeout, this, &AssassinModel::resetInvisibility);
}

///
/// \brief Executes the Assassin's primary attack, throwing a knife projectile in the current facing direction.
///
void AssassinModel::attack1() {
    if (canAttack1) {
        int attackDirectionX = 0, attackDirectionY = 0;

        // Determine the attack trajectory based on the player's current facing direction
        switch(itsDirection) {
        case Left :     attackDirectionX = -1; break;
        case Right:     attackDirectionX = 1;  break;
        case Up:        attackDirectionY = -1; break;
        case Down:      attackDirectionY = 1;  break;
        case LeftDown:  attackDirectionX = -1; attackDirectionY = 1;  break;
        case LeftUp:    attackDirectionX = -1; attackDirectionY = -1; break;
        case RightDown: attackDirectionX = 1;  attackDirectionY = 1;  break;
        case RightUp:   attackDirectionX = 1;  attackDirectionY = -1; break;
        default: break;
        }

        QLineF aimLine(QPointF(0,0), QPointF(attackDirectionX, attackDirectionY));

        // Knife hitbox dimensions
        QRectF knifeHitbox(0, 0, 40, 20);
        knifeHitbox.moveCenter(itsHitBox.center());

        // Creation of the Knife projectile
        ProjectileModel* knife = new ProjectileModel(
            knifeHitbox,
            aimLine,
            8, // Speed of 8 px/tick
            itsAttack1Damage, // Damage: 7 hp
            PLAYER,
            KNIFE
            );

        emit fireProjectile(knife);

        canAttack1 = false;
        itsCooldown1->start(AssassinAttack1Cooldown * 1000);
    }
}

///
/// \brief Executes the Assassin's secondary ability, deploying a smoke bomb to become invisible.
///
void AssassinModel::attack2() {
    if (canAttack2) {
        isInvisible = true; // Activate the invisibility effect
        smokeBombTimer->start(AssassinAttack2Duration * 1000);

        canAttack2 = false;
    }
}

///
/// \brief Checks if the Assassin is currently under the invisibility effect.
/// \return True if the Assassin is invisible, false otherwise.
///
bool AssassinModel::getIsInvisible() const
{
    return isInvisible;
}

///
/// \brief Ends the invisibility effect and starts the cooldown timer for the smoke bomb ability.
///
void AssassinModel::resetInvisibility() {
    isInvisible = false;
    itsCooldown2->start(AssassinAttack2Cooldown * 1000);
}
