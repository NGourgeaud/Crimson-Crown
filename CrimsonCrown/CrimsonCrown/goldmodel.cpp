// File Name: goldmodel.cpp
// Group: A11
// Description: Model class managing the player's gold currency, allowing addition, removal, and balance checks.

#include "goldmodel.h"
#include "qdebug.h"

///
/// \brief Retrieves the current amount of gold held by the player.
/// \return An integer representing the total gold balance.
///
int GoldModel::getItsGold() const
{
    return itsGold;
}

///
/// \brief Adds a specified amount of gold to the current balance.
/// \param amount The amount of gold to add.
///
void GoldModel::addGold(int amount)
{
    itsGold += amount;
    // Debug prints kept for development monitoring
    qDebug() << "added " << amount << "gold";
    qDebug() << "Current Gold : " << itsGold;
}

///
/// \brief Deducts a specified amount of gold, ensuring the balance does not drop below zero.
/// \param amount The amount of gold to remove.
///
void GoldModel::removeGold(int amount)
{
    if (itsGold - amount <= 0){
        itsGold = 0;
    }
    else{
        itsGold -= amount;
    }
}

///
/// \brief Checks if the player has at least the specified amount of gold.
/// \param amount The required amount of gold to check against.
/// \return True if the player has enough gold, false otherwise.
///
bool GoldModel::hasEnoughGold(int amount)
{
    if (itsGold >= amount)
        return true;
    return false;
}

///
/// \brief Resets the gold balance back to zero.
///
void GoldModel::resetGold()
{
    itsGold = 0;
}

///
/// \brief Default constructor initializing the gold balance to zero.
///
GoldModel::GoldModel()
{
    itsGold = 0;
}
