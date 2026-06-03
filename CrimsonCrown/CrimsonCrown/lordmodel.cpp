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
    itsHitBox = QRect(x,y, LordWidth, LordHeight);
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

void LordModel::attack1(QPointF heroPos)
{
    QPainterPath * hitboxCercle = nullptr;
    hitboxCercle->addEllipse(itsHitBox.center().x(), itsHitBox.center().y(), LordAttack1HitBoxWidth, LordAttack1HitBoxHeight);
    canAttack1 = false;
    itsCoolDownTimer1->start(itsAttack1Cooldown*1000);
    emit attacking(hitboxCercle, itsAttack1Damage);
}

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

void LordModel::takeDamage(int amount)
{
    itsHP-= amount;
    if (itsHP<=0)
        emit death(this);
}

void LordModel::move(int axisX, int axisY)
{
    itsHitBox.setTopLeft(QPoint(axisX,axisY));
}

QRectF LordModel::getHitBox() const
{
    return itsHitBox;
}

bool LordModel::handleAI(int heroX, int heroY)
{
    if (QLineF(QPoint(heroX, heroY), itsHitBox.center()).length() > LordAttack1HitBoxWidth/2 and !isDashing){
        for (int i = 0; i<itsSpeed; i++){
            QLineF();
        }
        return false;
    }
    else{
        return true;
    }
}

void LordModel::resetAttack1Cooldown()
{
    canAttack1 = true;
    itsCoolDownTimer1->stop();
}

void LordModel::resetAttack2Cooldown()
{
    canAttack2 = true;
    itsCoolDownTimer2->stop();
}

void LordModel::dashStep()
{
    if (currentDashStep >= maxDashSteps) {
        itsDashTimer->stop();
        itsCoolDownTimer2->start(itsAttack2Cooldown*1000);
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
    emit dashing(attackHitbox);

    // On passe à l'étape suivante pour le prochain "tic" du timer
    currentDashStep++;
}
