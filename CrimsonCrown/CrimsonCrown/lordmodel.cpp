// File Name: lordmodel.cpp
// Group: A11
// Description: Model class for the Lord boss, handling health, dash attacks, and area-of-effect spin mechanics.

#include "lordmodel.h"
#include "config.h"
#include "qline.h"
#include "qtransform.h"
#include <QPainterPath>

///
/// \brief Initializes the Lord boss with health, speed, and specific attack timers.
/// \param maxHp The maximum and starting health points of the Lord.
/// \param speed The movement speed of the Lord.
/// \param x The starting X coordinate.
/// \param y The starting Y coordinate.
/// \param attack1Cooldown Base cooldown for the primary spin attack.
/// \param attack2Cooldown Base cooldown for the secondary dash attack.
/// \param attack1Damage Damage dealt by the primary attack.
/// \param attack2Damage Damage dealt by the secondary attack.
///
LordModel::LordModel(int maxHp, int speed, int x, int y, float attack1Cooldown, float attack2Cooldown, int attack1Damage, int attack2Damage)
{
    itsHP = maxHp;
    itsMaxHP = maxHp;
    itsSpeed = speed;
    itsHitBox = QRectF(x, y, LordWidth, LordHeight);
    itsAttack1Cooldown = attack1Cooldown;
    itsAttack2Cooldown = attack2Cooldown;
    itsAttack1Damage = attack1Damage;
    itsAttack2Damage = attack2Damage;
    itsCoolDownTimer1 = new QTimer();
    itsCoolDownTimer2 = new QTimer();
    canAttack1 = false;
    canAttack2 = false;
    itsDashTimer = new QTimer();
    isDashing = false;
    currentDashStep = 0;
    maxDashSteps = 0;
    IframeTimer = new QTimer();
    isInIframe = false;
    itsType = LORD;

    connect(itsCoolDownTimer1, &QTimer::timeout, this, &LordModel::resetAttack1Cooldown);
    connect(itsCoolDownTimer2, &QTimer::timeout, this, &LordModel::resetAttack2Cooldown);
    connect(itsDashTimer, &QTimer::timeout, this, &LordModel::dashStep);
    connect(IframeTimer, &QTimer::timeout, this, &LordModel::resetIframe);
    itsCoolDownTimer1->start(1500);
    itsCoolDownTimer2->start(1500);
}

///
/// \brief Updates the Lord's logic per frame, triggering attacks based on the distance to the player.
/// \param heroPos The current position of the player's hero.
///
void LordModel::update(QPointF heroPos)
{
    if(handleAI(heroPos))
    {
        if(QLineF(heroPos, itsHitBox.center()).length() < LordAttack1HitBoxWidth - 25 && canAttack1){
            attack1(heroPos);
        }
    }
    if (QLineF(heroPos, itsHitBox.center()).length() > LordAttack1HitBoxWidth + 100 && canAttack2){
        attack2(heroPos);
    }
}

///
/// \brief Executes the primary spin attack of the Lord.
/// \param heroPos The current position of the player's hero (unused here).
///
void LordModel::attack1(QPointF heroPos)
{
    Q_UNUSED(heroPos);

    // Fixed Memory Leak: Allocated on the stack instead of using new without delete
    QPainterPath hitboxCercle;
    hitboxCercle.addEllipse(itsHitBox.center().x() - LordAttack1HitBoxWidth / 2, itsHitBox.center().y() - LordAttack1HitBoxHeight / 2, LordAttack1HitBoxWidth, LordAttack1HitBoxHeight);

    canAttack1 = false;
    QPolygon attackHitbox = hitboxCercle.toFillPolygon().toPolygon();
    itsCoolDownTimer1->start(itsAttack1Cooldown * 1000);
    emit attacking(HitZoneModel(attackHitbox, UNDEFINEDE), itsAttack1Damage);
}

///
/// \brief Initiates the secondary dash attack towards the player.
/// \param heroPos The current position of the player's hero.
///
void LordModel::attack2(QPointF heroPos)
{
    QLineF aimLine(itsHitBox.center(), heroPos);

    currentAimLine = aimLine;
    currentDashStep = 0;
    maxDashSteps = aimLine.length() / 5; // 5 pixels per step

    // Triggers the timer every 8 ms to simulate smooth rapid movement
    itsDashTimer->start(8);
    canAttack2 = false;
    isDashing = true;
}

///
/// \brief Deducts health from the Lord and activates invincibility frames.
/// \param amount The amount of damage points to deduct.
///
void LordModel::takeDamage(int amount)
{
    if (!isInIframe){
        itsHP -= amount;
        isInIframe = true;
        IframeTimer->start(LordIframe * 1000);
    }
    if (itsHP <= 0)
        emit death(this);
}

///
/// \brief Moves the Lord's hitbox to a specific top-left coordinate.
/// \param pos The new target coordinate.
///
void LordModel::move(QPointF pos)
{
    itsHitBox.moveTo(pos);
}

///
/// \brief Retrieves the current geometric bounding box of the Lord.
/// \return A QRectF representing the Lord's physical space.
///
QRectF LordModel::getHitBox() const
{
    return itsHitBox;
}

///
/// \brief Handles the Lord's AI logic, chasing the player if not currently dashing.
/// \param heroPos The current position of the player's hero.
/// \return True if the Lord is ready to attack (in range), false if it is moving.
///
bool LordModel::handleAI(QPointF heroPos)
{
    if (QLineF(heroPos, itsHitBox.center()).length() > LordAttack1HitBoxWidth / 2 && !isDashing){
        for (int i = 0; i < itsSpeed; i++){
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
/// \brief Resets the primary attack cooldown timer.
///
void LordModel::resetAttack1Cooldown()
{
    canAttack1 = true;
    itsCoolDownTimer1->stop();
}

///
/// \brief Resets the secondary attack cooldown timer.
///
void LordModel::resetAttack2Cooldown()
{
    canAttack2 = true;
    itsCoolDownTimer2->stop();
}

///
/// \brief Processes each step of the active dash attack, moving the Lord and emitting damage hitboxes.
///
void LordModel::dashStep()
{
    if (currentDashStep >= maxDashSteps) {
        itsDashTimer->stop();
        itsCoolDownTimer2->start(itsAttack2Cooldown * 1000);
        isDashing = false;
        return;
    }

    // Calculating the progression (from 0.0 to 1.0)
    qreal progression = (qreal)currentDashStep / (qreal)maxDashSteps;

    // Find the exact point on the attack line according to the progression and move the Lord
    QPointF nouvellePosition = currentAimLine.pointAt(progression);
    itsHitBox.moveCenter(nouvellePosition.toPoint());

    // Create the hitbox at this new position
    QRect baseAttackRect(0 / 2, -LancerAttackHitboxHeight / 2, LordAttack2HitBoxWidth, LancerAttackHitboxHeight);
    qreal angle = currentAimLine.angle(); // Returns the angle in degrees (0 is right, 90 is up)

    QTransform transform;
    transform.translate(itsHitBox.center().x(), itsHitBox.center().y());
    transform.rotate(-angle);

    QPolygon attackHitbox = transform.mapToPolygon(baseAttackRect);
    emit attacking(HitZoneModel(attackHitbox, SWORD), itsAttack2Damage);

    // We move on to the next step for the next "tick" of the timer.
    currentDashStep++;
}

///
/// \brief Disables the invincibility state once the i-frame timer finishes.
///
void LordModel::resetIframe()
{
    IframeTimer->stop();
    isInIframe = false;
}
