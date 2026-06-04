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
    bool isCorridor;
    bool isBossRoom;
    QRect itsWalls;

public:
    RoomModel(bool corridor = false, bool boosRoom = false, int width = 0, int height = 0);
    ~RoomModel();

    void lockDoors();
    void unlockDoors();

    // Getters
    const QList<ObstacleModel*>& getItsObstacles() const;
    bool getIsLocked() const;
    bool getIsCorridor() const;
    bool getIsBossRoom() const;
    int getRemainingEnemyCount() const;
    QRect getItsWalls() const;

    // Méthode utilitaire ajoutée pour faciliter la checklist "the placement of obstacles"
    void addObstacle(int x, int y, int width, int height);
};

#endif // ROOMMODEL_H
