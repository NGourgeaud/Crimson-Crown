#include "assassinmodel.h"
#include "config.h"

AssassinModel::AssassinModel()
    : PlayerModel(18, 18, 12, 7, 0, HeroType::Assassin)
{}

void AssassinModel::attack1() {
    if (canAttack1) {
        int attackDirectionX = 0, attackDirectionY = 0;

        switch(itsDirection) {
        case Left : attackDirectionX = -5; break;
        case Right: attackDirectionX = 5; break;
        case Up: attackDirectionY = -5; break;
        case Down: attackDirectionY = 5; break;
        case LeftDown: attackDirectionX = -5; attackDirectionY = 5; break;
        case LeftUp: attackDirectionX = -5; attackDirectionY = -5; break;
        case RightDown: attackDirectionX = 5; attackDirectionY = 5; break;
        case RightUp: attackDirectionX = 5; attackDirectionY = -5; break;
        default: break;
        }

        QLineF aimLine(QPointF(0,0), QPointF(attackDirectionX, attackDirectionY));
        qreal angle = aimLine.angle();

        QRect baseAttackRect(0, -AssassinAttack1HitboxHeight / 2, AssassinAttack1HitboxWidth, AssassinAttack1HitboxHeight);
        QTransform transform;
        transform.translate(itsHitBox.center().x(), itsHitBox.center().y());
        transform.rotate(-angle);

        QPolygon attackHitbox = transform.mapToPolygon(baseAttackRect);
        emit attackPlayer(attackHitbox, itsAttack1Damage);

        canAttack1 = false;
        itsCooldown1->start(AssassinAttack1Cooldown * 1000);
    }
}

void AssassinModel::attack2() {

}