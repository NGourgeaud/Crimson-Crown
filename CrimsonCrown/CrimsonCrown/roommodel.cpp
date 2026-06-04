#include "roommodel.h"

RoomModel::RoomModel(RoomType type, int x, int y, int w, int h)
    : isLocked(false), remainingEnemyCount(0),
    itsType(type), itsBounds(x, y, w, h)
{
    isCorridor = (type == CorridorHorizontal || type == CorridorVertical);
    isBossRoom = (type == Boss);
}

RoomModel::~RoomModel()
{
    qDeleteAll(itsObstacles);
    itsObstacles.clear();
}

void RoomModel::lockDoors() { isLocked = true; }
void RoomModel::unlockDoors() { isLocked = false; }
void RoomModel::spawnEnemies(int count) { remainingEnemyCount = count; }

const QList<ObstacleModel*>& RoomModel::getItsObstacles() const
{
    return itsObstacles;
}
bool RoomModel::getIsLocked() const
{
    return isLocked;
}

bool RoomModel::getIsCorridor() const
{
    return isCorridor;
}

bool RoomModel::getIsBossRoom() const
{
    return isBossRoom;
}

int RoomModel::getRemainingEnemyCount() const
{
    return remainingEnemyCount;
}

QRectF RoomModel::getBounds() const
{
    return itsBounds;
}

void RoomModel::addObstacle(int x, int y, int width, int height)
{
    itsObstacles.append(new ObstacleModel(x, y, width, height));
}