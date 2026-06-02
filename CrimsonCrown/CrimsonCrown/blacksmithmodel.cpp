#include "blacksmithmodel.h"
#include <QPainterPath>

BlacksmithModel::BlacksmithModel()
    :PlayerModel(40, 100, 4, 13, 10)
{}

void BlacksmithModel::attack1()
{
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

    QLineF aimLine(itsHitBox.center(), QPoint(attackDirectionX, attackDirectionY));

    qreal angle = aimLine.angle(); // Returns the angle in degrees (0 is right, 90 is up)

    // 2. Create the base attack rectangle (centered at 0,0 to make rotation easy)

    QRect baseAttackRect(itsHitBox.center().x(),itsHitBox.center().y(), BlacksmithAttack1HitboxWidth, BlacksmithAttack1HitboxHeight);

    // 3. Set up a QTransform to move and rotate the rectangle

    QTransform transform;

    transform.rotate(-angle); // Rotate towards Hero (Qt angles go counter-clockwise, QTransform rotates clockwise, so we negate)

    // 4. Map the rectangle through the transform to get a rotated polygon

    QPolygon attackHitbox = transform.mapToPolygon(baseAttackRect);

    emit attackPlayer(attackHitbox, itsAttack1Damage);

    canAttack1 = false;
    itsCooldown1->start(BlacksmithAttack1Cooldown*1000);
}

void BlacksmithModel::attack2()
{
    // Créer un chemin mathématique (QPainterPath)
    QPainterPath path;

    // addEllipse prend le centre et les rayons X et Y.
    // Mettre la même valeur crée un cercle parfait.
    path.addEllipse(itsHitBox.center(), BlacksmithAttack2HitboxRadius, BlacksmithAttack2HitboxRadius);

    // Convertir ce chemin en polygone
    // toFillPolygon() renvoie un QPolygonF (avec des floats)
    // toPolygon() le convertit en QPolygon (avec des entiers) pour correspondre au signal
    QPolygon attackHitbox = path.toFillPolygon().toPolygon();

    // Émettre le signal avec la nouvelle zone d'attaque
    emit attackPlayer(attackHitbox, itsAttack2Damage);

    canAttack2 = false;
    itsCooldown2->start(BlacksmithAttack2Cooldown*1000);
}


