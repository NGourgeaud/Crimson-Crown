// File Name: enemymodel.cpp
// Group: A11
// Description: Base model class for enemy entities, providing access to generic attributes like type and speed.

#include "enemymodel.h"

///
/// \brief Default constructor for the base EnemyModel.
///
EnemyModel::EnemyModel() {}

///
/// \brief Retrieves the specific type of the enemy.
/// \return The EnemyType enumerator representing this enemy.
///
EnemyType EnemyModel::getitsEnemyType()
{
    return itsType;
}

///
/// \brief Retrieves the movement speed of the enemy.
/// \return A float representing the speed value.
///
float EnemyModel::getitsSpeed()
{
    return itsSpeed;
}
