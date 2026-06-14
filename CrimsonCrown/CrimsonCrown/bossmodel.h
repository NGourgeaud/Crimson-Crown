// File Name: bossmodel.h
// Group: A11
// Description: Header file defining the abstract base class for all boss entities.

#ifndef BOSSMODEL_H
#define BOSSMODEL_H

#include "config.h"
#include "entitymodel.h"
#include "qtimer.h"

class BossModel : public EntityModel
{
protected:
    float itsAttack1Cooldown;
    float itsAttack2Cooldown;
    int itsAttack1Damage;
    int itsAttack2Damage;
    QTimer * IframeTimer;
    bool isInIframe;
    BossType itsType;
    float itsSpeed;

public:
    ///
    /// \brief Default constructor.
    ///
    BossModel();

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~BossModel();

    ///
    /// \brief Pure virtual function to define the first boss attack.
    /// \param heroPos The position of the player's hero.
    ///
    virtual void attack1(QPointF heroPos) = 0;

    ///
    /// \brief Pure virtual function to define the second boss attack.
    /// \param heroPos The position of the player's hero.
    ///
    virtual void attack2(QPointF heroPos) = 0;

    ///
    /// \brief Pure virtual function for the boss's logic tick.
    /// \param heroPos The position of the player's hero.
    ///
    virtual void update(QPointF heroPos) = 0;

    ///
    /// \brief Retrieves the specific type of the boss.
    /// \return The BossType enumerator.
    ///
    BossType getItsType() const;

    ///
    /// \brief Retrieves the movement speed of the boss.
    /// \return The speed as a float.
    ///
    float getItsSpeed() const;
};

#endif // BOSSMODEL_H
