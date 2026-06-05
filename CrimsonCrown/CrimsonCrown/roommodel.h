#ifndef ROOMMODEL_H
#define ROOMMODEL_H

#include <QList>
#include <QRectF>
#include "obstaclemodel.h"

enum RoomType { Spawn, CorridorHorizontal, CorridorVertical, Combat, Boss };

class RoomModel
{
private:
    bool isLocked;// block the doors
    QList<ObstacleModel*> itsObstacles;
    bool isCorridor; // corridor
    bool isBossRoom; // boss

    RoomType itsType;
    QRectF itsBounds;

    bool isVisited;

public:
    RoomModel(RoomType type, int x, int y, int w, int h);
    ~RoomModel();

    void lockDoors();
    void unlockDoors();

    // Getters
    const QList<ObstacleModel*>& getItsObstacles() const;
    bool getIsLocked() const;
    bool getIsCorridor() const;
    bool getIsBossRoom() const;
    QRectF getBounds() const;

    RoomType getType() const { return itsType; }
    int getOffsetX() const { return itsBounds.x(); }
    int getOffsetY() const { return itsBounds.y(); }
    int getWidth() const { return itsBounds.width(); }
    int getHeight() const { return itsBounds.height(); }

    void addObstacle(int x, int y, int width, int height);
    bool getIsVisited() const;
    void setIsVisited();
};

#endif
