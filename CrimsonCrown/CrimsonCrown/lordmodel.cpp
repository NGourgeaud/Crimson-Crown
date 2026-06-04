#include "lordmodel.h"
#include "config.h"
#include "qline.h"
#include "qtransform.h"
#include <QPainterPath>


LordModel::LordModel(int maxHp, int speed, int x, int y, float attack1Cooldown, float attack2Cooldown, int attack1Damage, int attack2Damage)
{
    itsHP = maxHp;
    itsMaxHP = maxHp;
    itsSpeed = speed;
    itsHitBox = QRectF(x,y, LordWidth, LordHeight);
    itsAttack1Cooldown = attack1Cooldown;
    itsAttack2Cooldown = attack2Cooldown;
    itsAttack1Damage = attack1Damage;
    itsAttack2Damage = attack2Damage;
    itsCoolDownTimer1 = new QTimer();
    itsCoolDownTimer2 = new QTimer();
    canAttack1 = true;
    canAttack2 = true;
    itsDashTimer = new QTimer();
    isDashing = false;
    currentDashStep = 0;
    maxDashSteps = 0;

    connect(itsCoolDownTimer1, &QTimer::timeout, this, &LordModel::resetAttack1Cooldown);
    connect(itsCoolDownTimer2, &QTimer::timeout, this, &LordModel::resetAttack2Cooldown);
    connect(itsDashTimer, &QTimer::timeout, this, &LordModel::dashStep);
}

///
/// \brief Called to update the Lord
/// \param heroPos: QPointF of the hero's position
///
void LordModel::update(QPointF heroPos)
{
    if(handleAI(heroPos))
    {
        if(QLineF(heroPos, itsHitBox.center()).length() < LordAttack1HitBoxWidth-25 and canAttack1){
            attack1(heroPos);
        }
    }
    if (QLineF(heroPos, itsHitBox.center()).length() > LordAttack1HitBoxWidth+100 and canAttack2){
        attack2(heroPos);
    }

}

///
/// \brief First attack of the Lors (spin)
/// \param heroPos: QPointF of the hero's position
///
void LordModel::attack1(QPointF heroPos)
{
    Q_UNUSED(heroPos);
    QPainterPath * hitboxCercle = nullptr;
    hitboxCercle->addEllipse(itsHitBox.center().x(), itsHitBox.center().y(), LordAttack1HitBoxWidth, LordAttack1HitBoxHeight);
    canAttack1 = false;
    QPolygon attackHitbox = hitboxCercle->toFillPolygon().toPolygon();
    itsCoolDownTimer1->start(itsAttack1Cooldown*1000);
    emit attacking(&attackHitbox, itsAttack1Damage);
}

///
/// \brief Secondary attack of the Lord (dash)
/// \param heroPos: QPointF of the hero's position
///
void LordModel::attack2(QPointF heroPos)
{

    QLineF aimLine(itsHitBox.center(), heroPos);

    currentAimLine = aimLine;
    currentDashStep = 0;
    maxDashSteps = aimLine.length() / 5; // 5 pixels par étape

    // Déclenche le timer toutes les 8 ms
    itsDashTimer->start(8);
    canAttack2 = false;
    isDashing = true;
}

///
/// \brief Damages the Lord of the ammount
/// \param amount: amout of the damage the Lord will take
///
void LordModel::takeDamage(int amount)
{
    itsHP-= amount;
    if (itsHP<=0)
        emit death(this);
}

///
/// \brief Moves the Lord on pos
/// \param pos: QPointF of the position
///
void LordModel::move(QPointF pos)
{
    itsHitBox.moveCenter(pos);
}

///
/// \brief Returns the Lord's Hitbox
/// \return
///
QRectF LordModel::getHitBox() const
{
    return itsHitBox;
}

///
/// \brief Handles the Lord's AI movement
/// \param heroPos: QPointF of the hero's position
/// \return
///
bool LordModel::handleAI(QPointF heroPos)
{
    if (QLineF(heroPos, itsHitBox.center()).length() > LordAttack1HitBoxWidth/2 and !isDashing){
        for (int i = 0; i<itsSpeed; i++){
            QLineF aimLine = QLineF(itsHitBox.center(), heroPos);
            qreal angle = aimLine.angle();

            QLineF movementVector = QLineF::fromPolar(1, angle);

            itsHitBox.translate(movementVector.dx(), movementVector.dy());
        }
        return false;
    }
    else{
        return true;
    }
}

///
/// \brief Resets Attack1 cooldown
///
void LordModel::resetAttack1Cooldown()
{
    canAttack1 = true;
    itsCoolDownTimer1->stop();
}

///
/// \brief Resets Attack2 cooldown
///
void LordModel::resetAttack2Cooldown()
{
    canAttack2 = true;
    itsCoolDownTimer2->stop();
}

///
/// \brief Called to process each dash step
///
void LordModel::dashStep()
{
    if (currentDashStep >= maxDashSteps) {
        itsDashTimer->stop();
        itsCoolDownTimer2->start(itsAttack2Cooldown*1000);
        isDashing = false;
        return;
    }
    // Calcul de la progression (de 0.0 à 1.0)
    qreal progression = (qreal)currentDashStep / (qreal)maxDashSteps;

    // Trouve le point exact sur la ligne d'attaque selon la progression et déplace le Lord
    QPointF nouvellePosition = currentAimLine.pointAt(progression);
    itsHitBox.moveCenter(nouvellePosition.toPoint());

    // Création de la hitbox à cette nouvelle position
    QRect baseAttackRect(-LordAttack2HitBoxWidth / 2, -LancerAttackHitboxHeight / 2, LordAttack2HitBoxWidth, LancerAttackHitboxHeight);
    qreal angle = currentAimLine.angle(); // Returns the angle in degrees (0 is right, 90 is up)
    QTransform transform;
    transform.translate(itsHitBox.center().x(), itsHitBox.center().y());
    transform.rotate(-angle);

    QPolygon attackHitbox = transform.mapToPolygon(baseAttackRect);
    emit attacking(&attackHitbox, itsAttack2Damage);

    // On passe à l'étape suivante pour le prochain "tic" du timer
    currentDashStep++;
}
