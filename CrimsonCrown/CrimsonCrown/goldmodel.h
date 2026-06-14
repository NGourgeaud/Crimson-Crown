// File Name: goldmodel.h
// Group: A11
// Description: Header file defining the model managing the player's gold currency.

#ifndef GOLDMODEL_H
#define GOLDMODEL_H

class GoldModel
{
private:
    int itsGold;

public:
    ///
    /// \brief Constructs the gold model, initializing balance to zero.
    ///
    GoldModel();

    ///
    /// \brief Retrieves the current amount of gold.
    /// \return An integer representing the current gold balance.
    ///
    int getItsGold() const;

    ///
    /// \brief Adds gold to the current balance.
    /// \param amount The amount to add.
    ///
    void addGold(int amount);

    ///
    /// \brief Removes gold from the balance, capping at zero minimum.
    /// \param amount The amount to remove.
    ///
    void removeGold(int amount);

    ///
    /// \brief Checks if there is enough gold for a transaction.
    /// \param amount The required amount.
    /// \return True if the balance is sufficient, false otherwise.
    ///
    bool hasEnoughGold(int amount);

    ///
    /// \brief Resets the gold balance back to zero.
    ///
    void resetGold();
};

#endif // GOLDMODEL_H
