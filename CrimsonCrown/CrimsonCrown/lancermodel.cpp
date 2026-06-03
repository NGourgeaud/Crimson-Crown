#include "lancermodel.h"
#include "config.h"
#include "qline.h"
#include "qtransform.h"



LancerModel::LancerModel(int maxHp, int speed, int x, int y, float attackCooldown, int maxAttackDistance, int attackDamage) : EnemyModel()
{
    itsHP = maxHp;
    itsMaxHP = maxHp;
    itsSpeed = speed;
    itsHitBox = QRect(x,y,LancerWidth, LancerHeight);
    itsAttackCooldown = attackCooldown;
    itsMaxAttackDistance = maxAttackDistance;
    itsAttackDamage = attackDamage;
    canAttack = true;
    itsCoolDownTimer = new QTimer();

    connect(itsCoolDownTimer, &QTimer::timeout, this, &LancerModel::resetAttackCooldown);
}

///
/// \brief Updates the Lancer
/// \param heroX: x position of the center
/// \param heroY: y position of the center
///
void LancerModel::update(QPointF heroPos)
{
    if(handleAI(heroPos))
        attack(heroPos);
}

///
/// \brief Makes the Lancer attack the ennemy (sends a hitbox to GameModel to check for hits)
/// \param heroX: x position of the center
/// \param heroY: y position of the center
///
void LancerModel::attack(QPointF heroPos)
{
    // 1. Find the angle between the lancer and the hero
    QLineF aimLine(itsHitBox.center(), heroPos);
    qreal angle = aimLine.angle(); // Returns the angle in degrees (0 is right, 90 is up)

    // 2. Create the base attack rectangle (centered at 0,0 to make rotation easy)
    QRect baseAttackRect(itsHitBox.center().x(),itsHitBox.center().y(), LancerAttackHitboxWidth, LancerAttackHitboxHeight);

    // 3. Set up a QTransform to move and rotate the rectangle
    QTransform transform;
    transform.rotate(-angle); // Rotate towards Hero (Qt angles go counter-clockwise, QTransform rotates clockwise, so we negate)

    // 4. Map the rectangle through the transform to get a rotated polygon
    QPolygon attackHitbox = transform.mapToPolygon(baseAttackRect);
    emit attacking(attackHitbox);
    canAttack = false;
    itsCoolDownTimer->start(itsAttackCooldown*1000);
}

///
/// \brief Takes the hero position and processes the AI logic depending on it's position, also returns true if the ennemy can attack
/// \param heroX: x position of the center
/// \param heroY: y position of the center
///
bool LancerModel::handleAI(QPointF heroPos)
{
    // Goes towards the player if it's too far
    if (QLineF(heroPos, itsHitBox.center()).length() > LancerAttackHitboxWidth-20){
        for (int i = 0; i<itsSpeed; i++){
            // Goes to fastest towards the player
            int distX = std::abs(heroPos.x() - itsHitBox.center().x());
            int distY = std::abs(heroPos.y() - itsHitBox.center().y());

            if (distX + 20 >= distY or distX -20 <= distY or distY + 20 >= distX or distY <= distX){
                itsHitBox.translate(QPoint(itsHitBox.topLeft().x() + 1, itsHitBox.topLeft().y()+1));
            }
            else if (distX > distY){
                itsHitBox.translate(QPoint(itsHitBox.topLeft().x() + 1, itsHitBox.topLeft().y()));
            }
            else{
                itsHitBox.translate(QPoint(itsHitBox.topLeft().x(), itsHitBox.topLeft().y() + 1));
            }
        }
        return false;
    }
    else{
        return true;
    }
}

///
/// \brief Called to reset the atack cooldown
///
void LancerModel::resetAttackCooldown()
{
    canAttack = true;
    itsCoolDownTimer->stop();
}

///
/// \brief makes the Lancer take damage
/// \param amount: Amount of damage taken
/// \details if hp<=0 fires death signal
///
void LancerModel::takeDamage(int amount)
{
    itsHP-= amount;
    if (itsHP<=0)
        emit death();
}

///
/// \brief Moves the player at the set position (top left)
/// \param axisX: x center
/// \param axisY: y center
///
void LancerModel::move(QPointF pos)
{
    itsHitBox.setTopLeft(pos);
}

///
/// \brief Gets the Lancer's hitbox
/// \return Lancer's QRect
///
QRectF LancerModel::getHitBox() const
{
    return itsHitBox;
}
