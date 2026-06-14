// File Name: itemmodel.cpp
// Group: A11
// Description: Model class representing collectible items (like hearts or gold) dropped in the game.

#include "itemmodel.h"

///
/// \brief Retrieves the type of the item.
/// \return The ItemType enumerator representing this specific item (e.g., HEART, GOLD).
///
ItemType ItemModel::getItsType() const
{
    return itsType;
}

///
/// \brief Retrieves the physical bounding box of the item for collision detection.
/// \return A QRectF representing the item's geometric bounds.
///
QRectF ItemModel::getItsHitbox() const
{
    return itsHitbox;
}

///
/// \brief Constructs an ItemModel at a specific position with a defined type.
/// \param pos The coordinate where the item should be spawned.
/// \param type The specific type of the item (e.g., HEART, GOLD).
///
ItemModel::ItemModel(QPointF pos, ItemType type)
{
    itsHitbox = QRectF(pos, QSizeF(DropSize, DropSize));
    itsType = type;
}
