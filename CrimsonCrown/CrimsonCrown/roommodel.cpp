// File Name: roommodel.cpp
// Group: A11
// Description: Model class representing a dungeon room, managing its boundaries, locked state, and contained obstacles.

#include "roommodel.h"
#include "config.h"

///
/// \brief Constructs a room with a specific type, position, and dimensions.
/// \param type The RoomType enumerator (e.g., Spawn, Combat, Boss).
/// \param x The starting X coordinate.
/// \param y The starting Y coordinate.
/// \param w The width of the room.
/// \param h The height of the room.
///
RoomModel::RoomModel(RoomType type, int x, int y, int w, int h)
    : isLocked(false), itsType(type), itsBounds(x, y, w, h), isVisited(false)
{
    isCorridor = (type == CorridorHorizontal || type == CorridorVertical);
    isBossRoom = (type == Boss);
}

///
/// \brief Destructor that handles the cleanup of dynamically allocated obstacles.
///
RoomModel::~RoomModel()
{
    qDeleteAll(itsObstacles);
    itsObstacles.clear();
}

///
/// \brief Locks the room's doors to prevent the player from escaping during combat.
///
void RoomModel::lockDoors() { isLocked = true; }

///
/// \brief Unlocks the room's doors once combat is resolved.
///
void RoomModel::unlockDoors() { isLocked = false; }

///
/// \brief Retrieves the list of obstacles present in the room.
/// \return A constant reference to the QList of ObstacleModel pointers.
///
const QList<ObstacleModel*>& RoomModel::getItsObstacles() const
{
    return itsObstacles;
}

///
/// \brief Checks if the room is currently locked.
/// \return True if locked, false otherwise.
///
bool RoomModel::getIsLocked() const
{
    return isLocked;
}

///
/// \brief Checks if the room is classified as a corridor.
/// \return True if it is a corridor, false otherwise.
///
bool RoomModel::getIsCorridor() const
{
    return isCorridor;
}

///
/// \brief Checks if the room is designated for the boss encounter.
/// \return True if it is a boss room, false otherwise.
///
bool RoomModel::getIsBossRoom() const
{
    return isBossRoom;
}

///
/// \brief Retrieves the physical boundaries of the room.
/// \return A QRectF representing the room's area.
///
QRectF RoomModel::getBounds() const
{
    return itsBounds;
}

///
/// \brief Creates and adds a new static obstacle inside the room.
/// \param x The X coordinate of the obstacle relative to the room.
/// \param y The Y coordinate of the obstacle relative to the room.
/// \param width The width of the obstacle.
/// \param height The height of the obstacle.
///
void RoomModel::addObstacle(int x, int y, int width, int height)
{
    itsObstacles.append(new ObstacleModel(x, y, width, height));
}

///
/// \brief Checks if the player has already visited this room.
/// \return True if previously visited, false otherwise.
///
bool RoomModel::getIsVisited() const
{
    return isVisited;
}

///
/// \brief Marks the room as visited by the player.
///
void RoomModel::setIsVisited()
{
    isVisited = true;
}
