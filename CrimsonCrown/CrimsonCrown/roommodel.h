// File Name: roommodel.h
// Group: A11
// Description: Header file defining the model for a dungeon room, managing bounds and obstacles.

#ifndef ROOMMODEL_H
#define ROOMMODEL_H

#include <QList>
#include <QRectF>
#include "obstaclemodel.h"

///
/// \brief Enumeration of different types of rooms.
///
enum RoomType { Spawn, CorridorHorizontal, CorridorVertical, Combat, Boss, Exit };

class RoomModel
{
private:
    bool isLocked; // block the doors
    QList<ObstacleModel*> itsObstacles;
    bool isCorridor; // corridor
    bool isBossRoom; // boss

    RoomType itsType;
    QRectF itsBounds;

    bool isVisited;

public:
    ///
    /// \brief Constructs a room with a specific type and dimensions.
    /// \param type The RoomType.
    /// \param x The starting X coordinate.
    /// \param y The starting Y coordinate.
    /// \param w The width of the room.
    /// \param h The height of the room.
    ///
    RoomModel(RoomType type, int x, int y, int w, int h);

    ///
    /// \brief Destructor handling obstacle cleanup.
    ///
    ~RoomModel();

    /// \brief Locks the room's doors.
    void lockDoors();

    /// \brief Unlocks the room's doors.
    void unlockDoors();

    // Getters
    /// \brief Retrieves the list of obstacles inside the room.
    /// \return A reference to the list of obstacle pointers.
    const QList<ObstacleModel*>& getItsObstacles() const;

    /// \brief Checks if the doors are locked.
    /// \return True if locked.
    bool getIsLocked() const;

    /// \brief Checks if the room is a corridor.
    /// \return True if it is a corridor.
    bool getIsCorridor() const;

    /// \brief Checks if the room is a boss room.
    /// \return True if it is a boss room.
    bool getIsBossRoom() const;

    /// \brief Retrieves the physical bounds of the room.
    /// \return The QRectF representing the room area.
    QRectF getBounds() const;

    /// \brief Retrieves the room type.
    /// \return The RoomType enumerator.
    RoomType getType() const { return itsType; }

    /// \brief Retrieves the X offset.
    /// \return The X coordinate.
    int getOffsetX() const { return itsBounds.x(); }

    /// \brief Retrieves the Y offset.
    /// \return The Y coordinate.
    int getOffsetY() const { return itsBounds.y(); }

    /// \brief Retrieves the room's width.
    /// \return The width in pixels.
    int getWidth() const { return itsBounds.width(); }

    /// \brief Retrieves the room's height.
    /// \return The height in pixels.
    int getHeight() const { return itsBounds.height(); }

    ///
    /// \brief Creates and adds a new static obstacle inside the room.
    /// \param x The X coordinate relative to the room.
    /// \param y The Y coordinate relative to the room.
    /// \param width The width of the obstacle.
    /// \param height The height of the obstacle.
    ///
    void addObstacle(int x, int y, int width, int height);

    /// \brief Checks if the room has been visited.
    /// \return True if visited.
    bool getIsVisited() const;

    /// \brief Marks the room as visited.
    void setIsVisited(); };

#endif // ROOMMODEL_H
