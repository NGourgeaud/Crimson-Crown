#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "enemymodel.h"
#include <QObject>

class GameModel
{
private:
    QList<EnemyModel*> itsEnemies;
public:
    GameModel();
};

#endif // GAMEMODEL_H
