// File Name: enemymodel.h
// Group: A11
// Description: Header file defining the abstract base class for all standard enemies.

#ifndef ENEMYMODEL_H
#define ENEMYMODEL_H

#include "config.h"
#include "entitymodel.h"
#include "qtimer.h"

class EnemyModel : public EntityModel
{
    friend class TestEntity;
protected:
    float itsAttackCooldown;
    int itsAttackDamage;
    QTimer * IframeTimer;
    bool isInIframe;
    EnemyType itsType;
public:
    ///
    /// \brief Default constructor.
    ///
    EnemyModel();

    ///
    /// \brief Default virtual destructor.
    ///
    ~EnemyModel() = default;

    ///
    /// \brief Pure virtual function handling the enemy's attack logic.
    /// \param heroPos The position of the player's hero.
    ///
    virtual void attack(QPointF heroPos) = 0;

    ///
    /// \brief Pure virtual function handling the enemy's artificial intelligence.
    /// \param heroPos The position of the player's hero.
    /// \return True if the enemy is in position to attack, false otherwise.
    ///
    virtual bool handleAI(QPointF heroPos) = 0;

    ///
    /// \brief Pure virtual function to update the enemy's state per frame.
    /// \param heroPos The position of the player's hero.
    ///
    virtual void update(QPointF heroPos) = 0;

    ///
    /// \brief Retrieves the specific type of the enemy.
    /// \return The EnemyType enumerator.
    ///
    EnemyType getitsEnemyType();

    ///
    /// \brief Retrieves the enemy's movement speed.
    /// \return A float representing the speed.
    ///
    float getitsSpeed();
};

#endif // ENEMYMODEL_H
