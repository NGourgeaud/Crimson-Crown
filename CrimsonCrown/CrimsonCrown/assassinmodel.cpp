#include "assassinmodel.h"
#include "config.h"

AssassinModel::AssassinModel()
    : PlayerModel(18, 18, 12, 7, 0, HeroType::Assassin)
{
    smokeBombTimer = new QTimer(this);
    smokeBombTimer->setSingleShot(true);
    isInvisible = false;
    connect(smokeBombTimer, &QTimer::timeout, this, &AssassinModel::resetInvisibility);
}

void AssassinModel::attack1() {
    if (canAttack1) {
        int attackDirectionX = 0, attackDirectionY = 0;

        // Détermination de la direction
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

        // Hitbox du couteau
        QRectF knifeHitbox(0, 0, 40, 20);
        knifeHitbox.moveCenter(itsHitBox.center());

        // Création du projectile Couteau
        ProjectileModel* knife = new ProjectileModel(
            knifeHitbox,
            aimLine,
            8, // Vitesse de 8 px/tick comme sur ton image
            itsAttack1Damage, // Dégâts : 7 hp
            PLAYER,
            KNIFE
            );

        emit fireProjectile(knife);

        canAttack1 = false;
        itsCooldown1->start(AssassinAttack1Cooldown * 1000);
    }
}

void AssassinModel::attack2() {
    if (canAttack2) {
        isInvisible = true; // Activer l'effet
        smokeBombTimer->start(5000); // 5 secondes

        canAttack2 = false;
        itsCooldown2->start(10000); // Cooldown de la compétence (ajustable)
    }
}

bool AssassinModel::getIsInvisible() const
{
    return isInvisible;
}

void AssassinModel::resetInvisibility() {
    isInvisible = false;
}
