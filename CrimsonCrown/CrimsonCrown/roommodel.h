#ifndef ROOMMODEL_H
#define ROOMMODEL_H

#include <QList>
#include "obstaclemodel.h"

enum RoomType {
    Spawn,
    Combat,
    Boss,
    CorridorHorizontal,
    CorridorVertical
};

class RoomModel
{
private:
    bool isLocked;
    int remainingEnemyCount;
    QList<ObstacleModel*> itsObstacles;

    RoomType itsType;
    int offsetX;
    int offsetY;

    // --- NOUVEAUTÉ : La salle connait sa taille ! ---
    int itsWidth;
    int itsHeight;

public:
    // On met des tailles par défaut très grandes
    RoomModel(RoomType type, int x = 0, int y = 0, int width = 1200, int height = 800);
    ~RoomModel();

    void lockDoors();
    void unlockDoors();
    void spawnEnemies(int count);

    const QList<ObstacleModel*>& getItsObstacles() const;
    bool getIsLocked() const;
    RoomType getType() const;
    int getRemainingEnemyCount() const;
    int getOffsetX() const;
    int getOffsetY() const;

    // Nouveaux getters
    int getWidth() const;
    int getHeight() const;

    void addObstacle(int x, int y, int width, int height);
};

#endif // ROOMMODEL_H