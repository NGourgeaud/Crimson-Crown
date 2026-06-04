#include "blacksmithmodel.h"
#include <QPainterPath>

BlacksmithModel::BlacksmithModel()
    :PlayerModel(40, 100, 8, 13, 10, HeroType::Blacksmith)
{}

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

        // 2. Le rectangle démarre à "0" sur l'axe X (devant le point de pivot)
        QRect baseAttackRect(0, -BlacksmithAttack1HitboxHeight / 2, BlacksmithAttack1HitboxWidth, BlacksmithAttack1HitboxHeight);

        // 3. Transformation
        QTransform transform;
        transform.translate(itsHitBox.center().x(), itsHitBox.center().y());
        transform.rotate(-angle);

        // 4. Génération de la zone
        QPolygon attackHitbox = transform.mapToPolygon(baseAttackRect);

        emit attackPlayer(attackHitbox, itsAttack1Damage);

        canAttack1 = false;
        itsCooldown1->start(BlacksmithAttack1Cooldown*1000);
    }
}

void BlacksmithModel::attack2()
{
    if (canAttack2){
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
}


