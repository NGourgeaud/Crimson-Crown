// File Name: huntermodel.cpp
// Group: A11
// Description: Model class for the Hunter hero, managing ranged arrow attacks and a timed area-of-effect trap.

#include "huntermodel.h"
#include <QPainterPath>
#include "config.h"
#include "qdebug.h"

///
/// \brief Initializes the Hunter player character with its base stats and ability timers.
///
HunterModel::HunterModel()
    :PlayerModel(25, 25, 6, 10, 5, HeroType::Hunter)
{
    itsAttack2Timer = new QTimer;
    connect(itsAttack2Timer, &QTimer::timeout, this, &HunterModel::tickAttack2);
}

///
/// \brief Executes the Hunter's primary attack, firing an arrow projectile in the current facing direction.
///
void HunterModel::attack1()
{
    if (canAttack1){
        int attackDirectionX = 0, attackDirectionY = 0;

        // Determines the firing trajectory based on the player's direction
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
            10, // Arrow speed (adjustable)
            itsAttack1Damage,
            PLAYER,
            ARROW
            );

        emit fireProjectile(arrow);

        canAttack1 = false;
        itsCooldown1->start(HunterAttack1Cooldown * 1000);
    }
}

///
/// \brief Executes the Hunter's secondary ability, deploying a circular area-of-effect trap that deals damage over time.
///
void HunterModel::attack2()
{
    if (canAttack2){
        QPainterPath path;

        path.addEllipse(itsHitBox.center().x() - HunterAttack2HitboxRadius/2,
                        itsHitBox.center().y() - HunterAttack2HitboxRadius/2,
                        HunterAttack2HitboxRadius, HunterAttack2HitboxRadius);

        QPolygon attackHitbox = path.toFillPolygon().toPolygon();

        itsAttack2Area = attackHitbox;
        emit attackPlayer(HitZoneModel(attackHitbox, UNDEFINEDP), itsAttack2Damage);

        itsAttack2Time = 0;
        itsAttack2Timer->start(HunterAttack2TickRate * 1000);
        canAttack2 = false;
    }
}

///
/// \brief Processes the damage ticks for the secondary trap ability and stops it when the duration expires.
///
void HunterModel::tickAttack2()
{
    if (itsAttack2Time >= HunterAttack2Duration){
        itsCooldown2->start(HunterAttack2Cooldown * 1000);
        itsAttack2Timer->stop();
        return;
    }

    emit attackPlayer(HitZoneModel(itsAttack2Area, UNDEFINEDP), itsAttack2Damage);
    itsAttack2Time += HunterAttack2TickRate;

    // Debug prints kept for ability timing monitoring
    qDebug() << itsAttack2Time;
    qDebug() << HunterAttack2Duration;
}
