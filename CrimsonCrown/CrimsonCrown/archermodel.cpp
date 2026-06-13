// File Name: archermodel.cpp
// Group: A11
// Description: Model class representing the Archer enemy, handling its health, AI movement, and ranged attack logic.

#include "archermodel.h"
#include "qline.h"
#include <random>

///
/// \brief Initializes an Archer enemy with health, speed, and randomized attack timing.
/// \param maxHp The maximum and starting health points of the archer.
/// \param speed The movement speed of the archer.
/// \param x The starting X coordinate.
/// \param y The starting Y coordinate.
/// \param attackCooldown The base time in seconds between each attack.
/// \param attackDamage The amount of damage the archer's projectile deals.
///
ArcherModel::ArcherModel(int maxHp, int speed, int x, int y, float attackCooldown, int attackDamage)
{
    itsHP = maxHp;
    itsMaxHP = maxHp;
    itsSpeed = speed;
    itsHitBox = QRectF(x, y, LancerWidth, LancerHeight);
    itsAttackCooldown = attackCooldown;
    itsAttackDamage = attackDamage;
    canAttack = false;
    itsCoolDownTimer = new QTimer();
    IframeTimer = new QTimer();
    isInIframe = false;
    itsType = ARCHER;

    // Generates a random delay for the first shot so multiple archers don't fire simultaneously
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> firstShotTime(1,2);

    connect(itsCoolDownTimer, &QTimer::timeout, this, &ArcherModel::resetAttackCooldown);
    connect(IframeTimer, &QTimer::timeout, this, &ArcherModel::resetIframe);
    itsCoolDownTimer->start(firstShotTime(eng) * 1000);
}

///
/// \brief Updates the archer's logic per frame, handling movement and attacks.
/// \param heroPos The current position of the player's hero.
///
void ArcherModel::update(QPointF heroPos)
{
    if(handleAI(heroPos) && canAttack)
        attack(heroPos);
}

///
/// \brief Fires an arrow projectile towards the player's position and resets the cooldown.
/// \param heroPos The target position to aim the projectile at.
///
void ArcherModel::attack(QPointF heroPos)
{
    QLineF aimLine(itsHitBox.center(), heroPos);
    ProjectileModel * projectile = new ProjectileModel(QRectF(itsHitBox.center().x(), itsHitBox.center().y(), ArcherAttackHitboxWidth, ArcherAttackHitboxHeight), aimLine,
                                                      3, ArcherDefaultAttackDamage, ENEMY, ARROW);
    emit attacking(projectile);

    canAttack = false;
    itsCoolDownTimer->start(ArcherDefaultAttackCoolDown * 1000);
}

///
/// \brief Deducts health from the archer and activates invincibility frames.
/// \param amount The amount of damage points to deduct from the archer's health.
///
void ArcherModel::takeDamage(int amount)
{
    if (!isInIframe){
        itsHP -= amount;
        isInIframe = true; // Fixed: Now the archer actually enters the i-frame state
        IframeTimer->start(LancerIframe * 1000);
    }

    // Checks if the archer's health has depleted to trigger the death signal
    if (itsHP <= 0)
        emit death(this);
}

///
/// \brief Forces the archer's hitbox to move to a specific position.
/// \param pos The new coordinate to move the archer to.
///
void ArcherModel::move(QPointF pos)
{
    itsHitBox.moveTo(pos);
}

///
/// \brief Retrieves the current geometric bounding box of the archer.
/// \return A QRectF representing the archer's physical space and position.
///
QRectF ArcherModel::getHitBox() const
{
    return itsHitBox;
}

///
/// \brief Handles the archer's artificial intelligence, making it flee if the player gets too close.
/// \param heroPos The current position of the player's hero.
/// \return True if the archer is ready to attack (not fleeing), false otherwise.
///
bool ArcherModel::handleAI(QPointF heroPos)
{
    // Moves away from the player if they are within a 200 unit radius
    for (int i = 0; i < itsSpeed; i++){
        if (QLineF(heroPos, itsHitBox.center()).length() < 200){

            QLineF aimLine = QLineF(itsHitBox.center(), heroPos);
            qreal angle = aimLine.angle();

            QLineF movementVector = QLineF::fromPolar(-1, angle);
            itsHitBox.translate(movementVector.dx(), movementVector.dy());

            if (i == itsSpeed - 1)
                return false;
        }
    }
    return true;
}

///
/// \brief Allows the archer to attack again once the cooldown timer finishes.
///
void ArcherModel::resetAttackCooldown()
{
    canAttack = true;
    itsCoolDownTimer->stop();
}

///
/// \brief Disables the invincibility state once the i-frame timer finishes.
///
void ArcherModel::resetIframe()
{
    IframeTimer->stop();
    isInIframe = false;
}
