#ifndef ROOMMODEL_H
#define ROOMMODEL_H

#include <QList>
#include "obstaclemodel.h"

class RoomModel
{
private:
    bool isLocked;
    int remainingEnemyCount;
    QList<ObstacleModel*> itsObstacles; // Liste des obstacles de la salle

public:
    RoomModel();
    ~RoomModel();

    void lockDoors();
    void unlockDoors();
    void spawnEnemies(int count);

    // Getters
    const QList<ObstacleModel*>& getItsObstacles() const;
    bool getIsLocked() const;
    int getRemainingEnemyCount() const;

    // Méthode utilitaire ajoutée pour faciliter la checklist "the placement of obstacles"
    void addObstacle(int x, int y, int width, int height);
};

#endif // ROOMMODEL_H