#include "huntermodel.h"
#include <QPainterPath>

HunterModel::HunterModel()
    :PlayerModel(25, 25, 10, 10, 10, HeroType::Hunter) {}

void HunterModel::attack1()
{
    if (canAttack1){
        int attackDirectionX = 0, attackDirectionY = 0;

        switch(itsDirection) {
        case Left :     attackDirectionX = -5; break;
        case Right:     attackDirectionX = 5; break;
        case Up:        attackDirectionY = -5; break;
        case Down:      attackDirectionY = 5; break;
        case LeftDown:  attackDirectionX = -5; attackDirectionY = 5; break;
        case LeftUp:    attackDirectionX = -5; attackDirectionY = -5; break;
        case RightDown: attackDirectionX = 5;  attackDirectionY = 5; break;
        case RightUp:   attackDirectionX = 5;  attackDirectionY = -5; break;
        default: break;
        }

        QLineF aimLine(QPointF(0,0), QPointF(attackDirectionX, attackDirectionY));
        qreal angle = aimLine.angle();

        QRect baseAttackRect(0, -HunterAttack1HitboxHeight / 2, HunterAttack1HitboxWidth, HunterAttack1HitboxHeight);

        QTransform transform;
        transform.translate(itsHitBox.center().x(), itsHitBox.center().y());
        transform.rotate(-angle);

        QPolygon attackHitbox = transform.mapToPolygon(baseAttackRect);

        emit attackPlayer(attackHitbox, itsAttack1Damage);

        canAttack1 = false;
        itsCooldown1->start(HunterAttack1Cooldown * 1000);
    }
}

void HunterModel::attack2()
{
    if (canAttack2){
        QPainterPath path;

        path.addEllipse(itsHitBox.center().x() - HunterAttack2HitboxRadius/2,
                        itsHitBox.center().y() - HunterAttack2HitboxRadius/2,
                        HunterAttack2HitboxRadius, HunterAttack2HitboxRadius);

        QPolygon attackHitbox = path.toFillPolygon().toPolygon();

        emit attackPlayer(attackHitbox, itsAttack2Damage);

        canAttack2 = false;
        itsCooldown2->start(HunterAttack2Cooldown * 1000);
    }
}
