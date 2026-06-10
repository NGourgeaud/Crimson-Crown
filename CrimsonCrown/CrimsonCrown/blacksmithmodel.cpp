#include "blacksmithmodel.h"
#include <QPainterPath>

///
/// \brief Constructs a BlacksmithModel instance.
/// \details Initializes the player model with Blacksmith-specific base stats (HP, MaxHP, Speed, Attack1Damage, Attack2Damage, HeroType).
///
BlacksmithModel::BlacksmithModel()
    :PlayerModel(40, 40, 8, 13, 10, HeroType::Blacksmith)
{}

///
/// \brief Executes the primary attack of the Blacksmith.
/// \details Calculates the attack trajectory based on the player's current direction, creates a rotated rectangular hitbox, emits the attackPlayer signal, and starts the cooldown timer.
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

        // 2. The rectangle starts at "0" on the X-axis (in front of the pivot point)
        QRect baseAttackRect(0, -BlacksmithAttack1HitboxHeight / 2, BlacksmithAttack1HitboxWidth, BlacksmithAttack1HitboxHeight);

        // 3. Transformation
        QTransform transform;
        transform.translate(itsHitBox.center().x(), itsHitBox.center().y());
        transform.rotate(-angle);

        // 4. Zone generation
        QPolygon attackHitbox = transform.mapToPolygon(baseAttackRect);

        emit attackPlayer(HitZoneModel(attackHitbox, HAMMER), itsAttack1Damage);

        canAttack1 = false;
        itsCooldown1->start(BlacksmithAttack1Cooldown*1000);
    }
}

///
/// \brief Executes the secondary area-of-effect attack of the Blacksmith.
/// \details Generates a circular hitbox centered on the player using QPainterPath, converts it to a polygon, emits the attackPlayer signal, and starts the secondary attack cooldown.
///
void BlacksmithModel::attack2()
{
    if (canAttack2){
        // Create a mathematical path (QPainterPath)
        QPainterPath path;

        // addEllipse takes the center and the X and Y radii.
        // Setting the same value creates a perfect circle.
        path.addEllipse(itsHitBox.center().x() - BlacksmithAttack2HitboxRadius/2, itsHitBox.center().y() - BlacksmithAttack2HitboxRadius/2, BlacksmithAttack2HitboxRadius, BlacksmithAttack2HitboxRadius);

        // Convert this path to a polygon
        // toFillPolygon() returns a QPolygonF (with floats)
        // toPolygon() converts it to a QPolygon (with integers) to match the signal
        QPolygon attackHitbox = path.toFillPolygon().toPolygon();

        // Transmit the signal with the new attack zone
        emit attackPlayer(HitZoneModel(attackHitbox, UNDEFINEDP), itsAttack2Damage);

        canAttack2 = false;
        itsCooldown2->start(BlacksmithAttack2Cooldown*1000);
    }
}
