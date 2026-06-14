// File Name: bossmodel.cpp
// Group: A11
// Description: Base model class for boss entities, providing access to fundamental boss attributes like type and speed.

#include "bossmodel.h"

///
/// \brief Retrieves the specific type of the boss.
/// \return The BossType enumerator representing this boss.
///
BossType BossModel::getItsType() const
{
    return itsType;
}

///
/// \brief Retrieves the movement speed of the boss.
/// \return A float representing the speed value.
///
float BossModel::getItsSpeed() const
{
    return itsSpeed;
}

///
/// \brief Default constructor for the BossModel.
///
BossModel::BossModel() {}

///
/// \brief Destructor for the BossModel.
///
BossModel::~BossModel()
{

}
