#include "roommodel.h"

RoomModel::RoomModel(RoomType type, int x, int y, int width, int height)
    : isLocked(false), remainingEnemyCount(0), itsType(type), offsetX(x), offsetY(y), itsWidth(width), itsHeight(height)
{
}

RoomModel::~RoomModel()
{
    qDeleteAll(itsObstacles);
    itsObstacles.clear();
}

void RoomModel::lockDoors() { isLocked = true; }
void RoomModel::unlockDoors() { isLocked = false; }
void RoomModel::spawnEnemies(int count) { remainingEnemyCount = count; }

const QList<ObstacleModel*>& RoomModel::getItsObstacles() const { return itsObstacles; }
bool RoomModel::getIsLocked() const { return isLocked; }
RoomType RoomModel::getType() const { return itsType; }
int RoomModel::getRemainingEnemyCount() const { return remainingEnemyCount; }
int RoomModel::getOffsetX() const { return offsetX; }
int RoomModel::getOffsetY() const { return offsetY; }
int RoomModel::getWidth() const { return itsWidth; }
int RoomModel::getHeight() const { return itsHeight; }

void RoomModel::addObstacle(int x, int y, int width, int height)
{
    itsObstacles.append(new ObstacleModel(x, y, width, height));
}