// File Name: kingmodel.cpp
// Group: A11
// Description: Model class for the King boss, managing health, summoning minions, and multi-step area-of-effect jump attacks.

#include "kingmodel.h"
#include "qdebug.h"
#include "qline.h"
#include "qpainterpath.h"

///
/// \brief Initializes the King boss with health, speed, and complex attack timers.
/// \param maxHp The maximum and starting health points of the King.
/// \param speed The movement speed of the King.
/// \param x The starting X coordinate.
/// \param y The starting Y coordinate.
/// \param attack1Cooldown Base cooldown for the primary attack (summoning).
/// \param attack2Cooldown Base cooldown for the secondary attack (jump & AoE).
/// \param attack1Damage Damage dealt by the primary attack.
/// \param attack2Damage Damage dealt by the secondary attack.
///
KingModel::KingModel(int maxHp, int speed, int x, int y, float attack1Cooldown, float attack2Cooldown, int attack1Damage, int attack2Damage)
{
    itsHP = maxHp;
    itsMaxHP = maxHp;
    itsSpeed = speed;
    itsHitBox = QRectF(x, y, LordWidth, LordHeight);
    itsAttack1Cooldown = attack1Cooldown;
    itsAttack2Cooldown = attack2Cooldown;
    itsAttack1Damage = attack1Damage;
    itsAttack2Damage = attack2Damage;
    itsCoolDownTimer1 = new QTimer;
    itsCoolDownTimer2 = new QTimer;
    canAttack1 = false;
    canAttack2 = false;
    IframeTimer = new QTimer;
    isInIframe = false;
    itsSummoningTimer = new QTimer;
    isSummoning = false;
    itsjumpingTimer = new QTimer;
    itsAttack2Step2Timer = new QTimer;
    isJumping = false;
    canSummon = true;
    itsType = KING;

    connect(itsCoolDownTimer1, &QTimer::timeout, this, &KingModel::resetAttack1Cooldown);
    connect(itsCoolDownTimer2, &QTimer::timeout, this, &KingModel::resetAttack2Cooldown);
    connect(itsSummoningTimer, &QTimer::timeout, this, &KingModel::finishSummoning);
    connect(itsjumpingTimer, &QTimer::timeout, this, &KingModel::attack2Step1);
    connect(itsAttack2Step2Timer, &QTimer::timeout, this, &KingModel::attack2Step2);
    connect(IframeTimer, &QTimer::timeout, this, &KingModel::resetIframe);

    itsCoolDownTimer1->start(1500);
    itsCoolDownTimer2->start(1500);
}

///
/// \brief Updates the King's logic per frame, triggering attacks or summons based on distance to the player.
/// \param heroPos The current position of the player's hero.
///
void KingModel::update(QPointF heroPos)
{
    if (!isSummoning && !isJumping){
        if(handleAI(heroPos))
        {
            if (QLineF(heroPos, itsHitBox.center()).length() < 100 && canAttack2){
                attack2(heroPos);
                qDebug() << "Attack2 Requested";
            }
        }
        if(QLineF(heroPos, itsHitBox.center()).length() > 200 && canAttack1 && canSummon){
            attack1(heroPos);
            qDebug() << "Attack1 Requested";
        }
    }
}

///
/// \brief Deducts health from the King and activates invincibility frames.
/// \param amount The amount of damage points to deduct.
///
void KingModel::takeDamage(int amount)
{
    if (!isInIframe){
        itsHP -= amount;
        isInIframe = true;
        IframeTimer->start(LordIframe * 1000); // Note: Using LordIframe for the King
    }
    if (itsHP <= 0)
        emit death(this);
}

///
/// \brief Forces the King's hitbox to move to a specific position.
/// \param pos The new coordinate to move the King to.
///
void KingModel::move(QPointF pos)
{
    itsHitBox.moveTo(pos.toPoint());
}

///
/// \brief Retrieves the current geometric bounding box of the King.
/// \return A QRectF representing the King's physical space.
///
QRectF KingModel::getHitBox() const
{
    return itsHitBox;
}

///
/// \brief Handles the King's AI logic, chasing the player if they are too far away.
/// \param heroPos The current position of the player's hero.
/// \return True if the King is ready to attack (in range), false if it is moving.
///
bool KingModel::handleAI(QPointF heroPos)
{
    if (QLineF(heroPos, itsHitBox.center()).length() > LordAttack2HitBoxWidth - 10){
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
/// \brief Executes the primary attack phase (summoning minions).
/// \param heroPos The current position of the player's hero (unused here).
///
void KingModel::attack1(QPointF heroPos)
{
    Q_UNUSED(heroPos);
    canAttack1 = false;
    isSummoning = true;
    qDebug() << "Attack1 : Summon started";
    itsSummoningTimer->start(KingAttack1Duration * 1000);
}

///
/// \brief Initiates the secondary attack phase (the jump).
/// \param heroPos The current position of the player's hero (unused here).
///
void KingModel::attack2(QPointF heroPos)
{
    Q_UNUSED(heroPos);
    canAttack2 = false;
    isJumping = true;
    qDebug() << "Attack2 : jump started";
    itsjumpingTimer->start(KingJumpDuration * 1000);
}

///
/// \brief Updates the summoning capability status of the King.
/// \param newCanSummon Boolean indicating whether the King can summon new minions.
///
void KingModel::setCanSummon(bool newCanSummon)
{
    canSummon = newCanSummon;
}

///
/// \brief Completes the summoning phase, emits the spawn signal, and starts the cooldown.
///
void KingModel::finishSummoning()
{
    emit spawnEnemies();
    itsSummoningTimer->stop();
    isSummoning = false;
    qDebug() << "Attack1 : Summon done";
    itsCoolDownTimer1->start(KingDefaultAttack1CoolDown * 1000);
}

///
/// \brief Executes the landing phase of the jump, creating the primary center impact zone.
///
void KingModel::attack2Step1()
{
    itsjumpingTimer->stop();

    // Fixed Memory Leak: Allocated QPainterPath on the stack instead of dynamically on the heap without deletion.
    QPainterPath hitboxCercle;
    hitboxCercle.addEllipse(itsHitBox.center().x() - KingAttack2HitBoxWidth/2, itsHitBox.center().y() - KingAttack2HitBoxHeight/2, KingAttack2HitBoxWidth, KingAttack2HitBoxHeight);

    isJumping = false;
    qDebug() << "Attack2 : Jump done";
    qDebug() << "Attack2 : Step 1";

    QPolygon attackHitbox = hitboxCercle.toFillPolygon().toPolygon();
    itsAttack2Step2Timer->start(KingAttack2Step2Delay * 1000);
    emit attacking(HitZoneModel(attackHitbox, UNDEFINEDE), itsAttack2Damage);
}

///
/// \brief Executes the second phase of the jump attack, creating 8 outward shockwave impacts.
///
void KingModel::attack2Step2()
{
    itsAttack2Step2Timer->stop();
    qDebug() << "Attack2 : Step 2";

    for (int i = 0; i < 8; ++i) {
        // 1. Calculate the angle in radians (45 degrees = PI / 4)
        qreal angle = i * (M_PI / 4.0);

        // 2. Find the center of the small circular zone
        qreal zoneCenterX = itsHitBox.center().x() + 160 * qCos(angle);
        qreal zoneCenterY = itsHitBox.center().y() + 160 * qSin(angle);
        QPointF zoneCenter(zoneCenterX, zoneCenterY);

        // 3. Create the perfect circle using QPainterPath
        QPainterPath path;
        // addEllipse takes the center, X radius, and Y radius
        path.addEllipse(zoneCenter, 25, 25);

        emit attacking(HitZoneModel(path.toFillPolygon().toPolygon(), UNDEFINEDE), itsAttack2Damage);
        qDebug() << "Attack2 : Emit attack";
    }

    itsCoolDownTimer2->start(KingDefaultAttack2CoolDown * 1000);
}

///
/// \brief Resets the primary attack cooldown timer.
///
void KingModel::resetAttack1Cooldown()
{
    canAttack1 = true;
    itsCoolDownTimer1->stop();
    qDebug() << "Attack1 : cooldown reset";
}

///
/// \brief Resets the secondary attack cooldown timer.
///
void KingModel::resetAttack2Cooldown()
{
    canAttack2 = true;
    itsCoolDownTimer2->stop();
    qDebug() << "Attack2 : cooldown reset";
}

///
/// \brief Disables the invincibility state once the i-frame timer finishes.
///
void KingModel::resetIframe()
{
    IframeTimer->stop();
    isInIframe = false;
    qDebug() << "Iframe : cooldown reset";
}
