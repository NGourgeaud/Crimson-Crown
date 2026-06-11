#include "clergymodel.h"
#include "config.h"
#include "projectilemodel.h"
#include <QPainterpath>


clergyModel::clergyModel(int maxHp, int speed, int x, int y, float attack1Cooldown, float attack2Cooldown, int attack1Damage, int attack2Damage)
{
    itsHP = maxHp;
    itsMaxHP = maxHp;
    itsSpeed = speed;
    itsHitBox = QRectF(x,y, ClergyWidth, ClergyHeight);
    itsAttack1Cooldown = attack1Cooldown;
    itsAttack2Cooldown = attack2Cooldown;
    itsAttack1Damage = attack1Damage;
    itsAttack2Damage = attack2Damage;
    itsCoolDownTimer1 = new QTimer();
    itsCoolDownTimer2 = new QTimer();
    canAttack1 = false;
    canAttack2 = false;
    IframeTimer = new QTimer();
    isInIframe = false;

    connect(itsCoolDownTimer1, &QTimer::timeout, this, &clergyModel::resetAttack1Cooldown);
    connect(itsCoolDownTimer2, &QTimer::timeout, this, &clergyModel::resetAttack2Cooldown);
    connect(IframeTimer, &QTimer::timeout, this, &clergyModel::resetIframe);
    itsCoolDownTimer1->start(1500);
    itsCoolDownTimer2->start(1500);
}

///
/// \brief Called to update the Lord
/// \param heroPos: QPointF of the hero's position
///
void clergyModel::update(QPointF heroPos)
{
    handleAI(heroPos);

    // 2. On gère l'attaque 1
    if(canAttack1) {
        attack1(heroPos);
    }

    // 3. On gère l'attaque 2
    if (canAttack2) {
        attack2(heroPos);
    }

}

///
/// \brief First attack of the Lors (spin)
/// \param heroPos: QPointF of the hero's position
///
void clergyModel::attack1(QPointF heroPos)
{
    QLineF aimLine(itsHitBox.center(), heroPos);

    QRectF projHitbox(0, 0, ClergyAttack1HitBoxWidth, ClergyAttack1HitBoxHeight);
    projHitbox.moveCenter(itsHitBox.center());
    ProjectileModel * projectile = new ProjectileModel(projHitbox, aimLine, 3, ClergyDefaultAttack1Damage, ENEMY, RADIUS);

    emit attacking(projectile);
    canAttack1 = false;
    itsCoolDownTimer1->start(ClergyDefaultAttack1CoolDown * 1000);
}

///
/// \brief Secondary attack of the Lord (dash)
/// \param heroPos: QPointF of the hero's position
///
void clergyModel::attack2(QPointF heroPos)
{
    QLineF aimLine1(itsHitBox.center(), heroPos);

    QLineF aimLine2 = aimLine1;
    aimLine2.setAngle(aimLine1.angle() + 60); // Ajoute 60° par rapport à la ligne 1

    QLineF aimLine3 = aimLine2;
    aimLine3.setAngle(aimLine2.angle() + 60);

    QLineF aimLine4 = aimLine3;
    aimLine4.setAngle(aimLine3.angle() + 60);

    QLineF aimLine5 = aimLine4;
    aimLine5.setAngle(aimLine4.angle() + 60);

    QLineF aimLine6 = aimLine5;
    aimLine6.setAngle(aimLine5.angle() + 60);

    QRectF projHitbox(0, 0, ClergyAttack1HitBoxWidth, ClergyAttack1HitBoxHeight);
    projHitbox.moveCenter(itsHitBox.center());

    ProjectileModel * projectile1 = new ProjectileModel(projHitbox, aimLine1, 3, ClergyDefaultAttack1Damage, ENEMY, RADIUS);
    ProjectileModel * projectile2 = new ProjectileModel(projHitbox, aimLine2, 3, ClergyDefaultAttack1Damage, ENEMY, RADIUS);
    ProjectileModel * projectile3 = new ProjectileModel(projHitbox, aimLine3, 3, ClergyDefaultAttack1Damage, ENEMY, RADIUS);
    ProjectileModel * projectile4 = new ProjectileModel(projHitbox, aimLine4, 3, ClergyDefaultAttack1Damage, ENEMY, RADIUS);
    ProjectileModel * projectile5 = new ProjectileModel(projHitbox, aimLine5, 3, ClergyDefaultAttack1Damage, ENEMY, RADIUS);
    ProjectileModel * projectile6 = new ProjectileModel(projHitbox, aimLine6, 3, ClergyDefaultAttack1Damage, ENEMY, RADIUS);

    emit attacking(projectile1);
    emit attacking(projectile2);
    emit attacking(projectile3);
    emit attacking(projectile4);
    emit attacking(projectile5);
    emit attacking(projectile6);

    canAttack2 = false;
    itsCoolDownTimer2->start(ClergyDefaultAttack2CoolDown * 1000);
}

///
/// \brief Damages the Lord of the ammount
/// \param amount: amout of the damage the Lord will take
///
void clergyModel::takeDamage(int amount)
{
    if (!isInIframe){
        itsHP -= amount;
        isInIframe = true;
        IframeTimer->start(ClergyIframe * 1000);
    }
    if (itsHP<=0)
        emit death(this);
}

///
/// \brief Moves the Lord on pos
/// \param pos: QPointF of the position
///
void clergyModel::move(QPointF pos)
{
    itsHitBox.moveTo(pos);
}

///
/// \brief Returns the Lord's Hitbox
/// \return
///
QRectF clergyModel::getHitBox() const
{
    return itsHitBox;
}

///
/// \brief Handles the Lord's AI movement
/// \param heroPos: QPointF of the hero's position
/// \return
///
bool clergyModel::handleAI(QPointF heroPos)
{
    // Si le joueur est trop près (moins de 200 pixels)
    if (QLineF(heroPos, itsHitBox.center()).length() < 200) {

        QLineF aimLine(itsHitBox.center(), heroPos);

        // On crée un vecteur de mouvement:
        // Longueur = sa vitesse
        // Direction = Angle vers le joueur + 180 degrés (pour aller à l'opposé)
        QLineF movementVector = QLineF::fromPolar(itsSpeed, aimLine.angle() + 180);

        // On déplace la hitbox
        itsHitBox.translate(movementVector.dx(), movementVector.dy());

        return false;
    }

    return true;
}

///
/// \brief Resets Attack1 cooldown
///
void clergyModel::resetAttack1Cooldown()
{
    canAttack1 = true;
    itsCoolDownTimer1->stop();
}

///
/// \brief Resets Attack2 cooldown
///
void clergyModel::resetAttack2Cooldown()
{
    canAttack2 = true;
    itsCoolDownTimer2->stop();
}

void clergyModel::resetIframe()
{
    IframeTimer->stop();
    isInIframe = false;
}
