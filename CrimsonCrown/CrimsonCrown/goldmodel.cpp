#include "goldmodel.h"

int GoldModel::getItsGold() const
{
    return itsGold;
}

void GoldModel::addGold(int ammount)
{
    itsGold += ammount;
}

void GoldModel::removeGold(int ammount)
{
    if (itsGold - ammount <=0){
        itsGold = 0;
    }
    else{
        itsGold -= ammount;
    }
}

bool GoldModel::hasEnoughGold(int ammount)
{
    if (itsGold >= ammount)
        return true;
    return false;
}

void GoldModel::resetGold()
{
    itsGold = 0;
}

GoldModel::GoldModel()
{
    itsGold = 0;
}
