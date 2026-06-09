#include "huntermodel.h"
#include <QPainterPath>
#include "config.h"
#include "qdebug.h"

HunterModel::HunterModel()
    :PlayerModel(25, 25, 6, 10, 5, HeroType::Hunter)
{
    itsAttack2Timer = new QTimer;
    connect(itsAttack2Timer, &QTimer::timeout, this, &HunterModel::tickAttack2);
}

void HunterModel::attack1()
{
    if (canAttack1){
        int attackDirectionX = 0, attackDirectionY = 0;

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

        QRectF arrowHitbox(0, 0, 60, 15);
        arrowHitbox.moveCenter(itsHitBox.center());

        ProjectileModel* arrow = new ProjectileModel(
            arrowHitbox,
            aimLine,
            12, // Vitesse de la flèche (ajustable)
            itsAttack1Damage,
            PLAYER,
            ARROW
            );

        emit fireProjectile(arrow);

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

        itsAttack2Area = attackHitbox;
        emit attackPlayer(itsAttack2Area, itsAttack2Damage);
        itsAttack2Time = 0;
        itsAttack2Timer->start(HunterAttack2TickRate*1000);
        canAttack2 = false;

    }
}

void HunterModel::tickAttack2()
{
    if (itsAttack2Time >= HunterAttack2Duration){
        itsCooldown2->start(HunterAttack2Cooldown * 1000);
        itsAttack2Timer->stop();
        return;
    }
    emit attackPlayer(itsAttack2Area, itsAttack2Damage);
    itsAttack2Time += HunterAttack2TickRate;
    qDebug() << itsAttack2Time;
    qDebug() << HunterAttack2Duration;

}
