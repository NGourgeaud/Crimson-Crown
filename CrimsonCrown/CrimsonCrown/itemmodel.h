// File Name: itemmodel.h
// Group: A11
// Description: Header file defining the model for collectible items like health hearts and gold.

#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QObject>
#include "QRectF"
#include "config.h"

class ItemModel
{

private:
    QRectF itsHitbox;
    ItemType itsType;

public:
    ///
    /// \brief Constructs an item model at a given location.
    /// \param pos The drop position coordinates.
    /// \param type The specific type of item (e.g., HEART, GOLD).
    ///
    explicit ItemModel(QPointF pos, ItemType type);

    ///
    /// \brief Retrieves the item's type.
    /// \return The ItemType enumerator.
    ///
    ItemType getItsType() const;

    ///
    /// \brief Retrieves the physical hit box for item collection.
    /// \return A QRectF representing the collection area.
    ///
    QRectF getItsHitbox() const; };

#endif // ITEMMODEL_H
