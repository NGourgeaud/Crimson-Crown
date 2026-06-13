// File Name: blacksmithmodel.cpp
// Group: A11
// Description: Model class for the Blacksmith hero, managing stats, directional hammer attacks, and area-of-effect abilities.

#include "blacksmithmodel.h"
#include <QPainterPath>

///
/// \brief Initializes the Blacksmith player character with specific base stats (HP, speed, and damage).
///
BlacksmithModel::BlacksmithModel()
    :PlayerModel(40, 40, 8, 13, 10, HeroType::Blacksmith)
{}

///
/// \brief Executes the primary attack, creating a rotated rectangular hitbox based on the player's facing direction to simulate a hammer swing.
///
void BlacksmithModel::attack1()
{
    if (canAttack1){
        int attackDirectionX = 0, attackDirectionY = 0;

        switch(itsDirection) {
        case Left :
            attackDirectionX = -5;
            break;

        case Right:
            attackDirectionX = 5;
            break;
        case Up:
            attackDirectionY = -5;
            break;
        case Down:
            attackDirectionY = 5;
            break;
        case LeftDown:
            attackDirectionX = -5;
            attackDirectionY = 5;
            break;
        case LeftUp:
            attackDirectionX = -5;
            attackDirectionY = -5;
            break;
        case RightDown:
            attackDirectionX = 5;
            attackDirectionY = 5;
            break;
        case RightUp:
            attackDirectionX = 5;
            attackDirectionY = -5;
            break;
        default:
            break;
        }

        QLineF aimLine(QPointF(0,0), QPointF(attackDirectionX, attackDirectionY));
        qreal angle = aimLine.angle();

        // The rectangle starts at "0" on the X-axis (in front of the pivot point)
        QRect baseAttackRect(0, -BlacksmithAttack1HitboxHeight / 2, BlacksmithAttack1HitboxWidth, BlacksmithAttack1HitboxHeight);

        // Transforms the rectangle to rotate it according to the aiming angle
        QTransform transform;
        transform.translate(itsHitBox.center().x(), itsHitBox.center().y());
        transform.rotate(-angle);

        // Generates the final polygon for the attack zone
        QPolygon attackHitbox = transform.mapToPolygon(baseAttackRect);

        emit attackPlayer(HitZoneModel(attackHitbox, HAMMER), itsAttack1Damage);

        canAttack1 = false;
        itsCooldown1->start(BlacksmithAttack1Cooldown * 1000);
    }
}

///
/// \brief Executes the secondary area-of-effect attack, generating a circular hitbox around the player.
///
void BlacksmithModel::attack2()
{
    if (canAttack2){
        // Creates a mathematical path to draw the circular hitbox
        QPainterPath path;

        // addEllipse takes the top-left corner and the width/height to create a perfect circle around the center
        path.addEllipse(itsHitBox.center().x() - BlacksmithAttack2HitboxRadius / 2,
                        itsHitBox.center().y() - BlacksmithAttack2HitboxRadius / 2,
                        BlacksmithAttack2HitboxRadius,
                        BlacksmithAttack2HitboxRadius);

        // Converts the QPainterPath (QPolygonF) into a QPolygon (integers) to match the expected signal format
        QPolygon attackHitbox = path.toFillPolygon().toPolygon();

        // Transmits the signal with the new AoE attack zone
        emit attackPlayer(HitZoneModel(attackHitbox, UNDEFINEDP), itsAttack2Damage);

        canAttack2 = false;
        itsCooldown2->start(BlacksmithAttack2Cooldown * 1000);
    }
}
