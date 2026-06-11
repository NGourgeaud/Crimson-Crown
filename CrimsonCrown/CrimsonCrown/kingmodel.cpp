#include "kingmodel.h"
#include "qdebug.h"
#include "qline.h"
#include "qpainterpath.h"

KingModel::KingModel(int maxHp, int speed, int x, int y, float attack1Cooldown, float attack2Cooldown, int attack1Damage, int attack2Damage)
{
    itsHP = maxHp;
    itsMaxHP = maxHp;
    itsSpeed = speed;
    itsHitBox = QRectF(x,y, LordWidth, LordHeight);
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

void KingModel::update(QPointF heroPos)
{
    if (!isSummoning and !isJumping){
        if(handleAI(heroPos))
        {
            if (QLineF(heroPos, itsHitBox.center()).length() < 100 and canAttack2){
                attack2(heroPos);
                qDebug() << "Attack2 Requested";
            }
        }
        if(QLineF(heroPos, itsHitBox.center()).length() > 200 and canAttack1 and canSummon){
            attack1(heroPos);
            qDebug() << "Attack1 Requested";
        }
    }
}

void KingModel::takeDamage(int amount)
{
    if (!isInIframe){
        itsHP -= amount;
        isInIframe = true;
        IframeTimer->start(LordIframe*1000);
    }
    if (itsHP<=0)
        emit death(this);
}

void KingModel::move(QPointF pos)
{
    itsHitBox.moveTo(pos.toPoint());
}

QRectF KingModel::getHitBox() const
{
    return itsHitBox;
}

bool KingModel::handleAI(QPointF heroPos)
{
    if (QLineF(heroPos, itsHitBox.center()).length() > LordAttack2HitBoxWidth-10){
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

void KingModel::attack1(QPointF heroPos)
{
    Q_UNUSED(heroPos);
    canAttack1 = false;
    isSummoning = true;
    qDebug() << "Attack1 : Summon started";
    itsSummoningTimer->start(KingAttack1Duration*1000);
}

void KingModel::attack2(QPointF heroPos)
{
    Q_UNUSED(heroPos);
    canAttack2 = false;
    isJumping = true;
    qDebug() << "Attack2 : jump started";
    itsjumpingTimer->start(KingJumpDuration*1000);
}

void KingModel::setCanSummon(bool newCanSummon)
{
    canSummon = newCanSummon;
}

void KingModel::finishSummoning()
{
    emit spawnEnemies();
    itsSummoningTimer->stop();
    isSummoning = false;
    qDebug() << "Attack1 : Summon done";
    itsCoolDownTimer1->start(KingDefaultAttack1CoolDown*1000);
}

void KingModel::attack2Step1()
{
    itsjumpingTimer->stop();
    QPainterPath * hitboxCercle = new QPainterPath();
    hitboxCercle->addEllipse(itsHitBox.center().x() - KingAttack2HitBoxWidth/2, itsHitBox.center().y() - KingAttack2HitBoxHeight/2, KingAttack2HitBoxWidth, KingAttack2HitBoxHeight);
    isJumping = false;
    qDebug() << "Attack2 : Jump done";
    qDebug() << "Attack2 : Step 1";
    QPolygon attackHitbox = hitboxCercle->toFillPolygon().toPolygon();
    itsAttack2Step2Timer->start(KingAttack2Step2Delay*1000);
    emit attacking(HitZoneModel(attackHitbox, UNDEFINEDE), itsAttack1Damage);
}

void KingModel::attack2Step2()
{
    itsAttack2Step2Timer->stop();
    qDebug() << "Attack2 : Step 2";
    for (int i = 0; i < 8; ++i) {
        // 1. Calculer l'angle en radians (45 degrés = PI / 4)
        qreal angle = i * (M_PI / 4.0);

        // 2. Trouver le centre de la petite zone ronde
        qreal zoneCenterX = itsHitBox.center().x() + 160 * qCos(angle);
        qreal zoneCenterY = itsHitBox.center().y() + 160 * qSin(angle);
        QPointF zoneCenter(zoneCenterX, zoneCenterY);

        // 3. Créer le cercle parfait avec QPainterPath
        QPainterPath path;
        // addEllipse prend le centre, le rayon X, et le rayon Y
        path.addEllipse(zoneCenter, 25, 25);

        emit attacking(HitZoneModel(path.toFillPolygon().toPolygon(), UNDEFINEDE), itsAttack2Damage);
        qDebug() << "Attack2 : Emit attack";
    }

    itsCoolDownTimer2->start(KingDefaultAttack2CoolDown*1000);
}

void KingModel::resetAttack1Cooldown()
{
    canAttack1 = true;
    itsCoolDownTimer1->stop();
    qDebug() << "Attack1 : cooldown reset";
}


void KingModel::resetAttack2Cooldown()
{
    canAttack2 = true;
    itsCoolDownTimer2->stop();
    qDebug() << "Attack2 : cooldown reset";
}

void KingModel::resetIframe()
{
    IframeTimer->stop();
    isInIframe = false;
    qDebug() << "Iframe : cooldown reset";
}
