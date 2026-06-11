#ifndef GOLDMODEL_H
#define GOLDMODEL_H

class GoldModel
{
private:
    int itsGold;
public:
    GoldModel();
    int getItsGold() const;
    void addGold(int ammount);
    void removeGold(int ammount);
    bool hasEnoughGold(int ammount);
    void resetGold();
};

#endif // GOLDMODEL_H
