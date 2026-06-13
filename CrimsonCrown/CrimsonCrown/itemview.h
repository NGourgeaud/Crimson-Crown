// File Name: itemview.h
// Group: A11
// Description: Header file defining the visual rendering component for dropped collectible items.

#ifndef ITEMVIEW_H
#define ITEMVIEW_H

#include "config.h"
#include "itemmodel.h"
#include "qpixmap.h"
#include "QPainter"

class ItemView
{
private:
    QPixmap itsTexture;
public:
    ///
    /// \brief Default constructor.
    ///
    ItemView();

    ///
    /// \brief Renders the item onto the game screen.
    /// \param painter Pointer to the QPainter.
    /// \param model Pointer to the ItemModel containing data to draw.
    ///
    void draw(QPainter * painter, ItemModel * model);

    ///
    /// \brief Loads the visual sprite corresponding to the item type.
    /// \param type The specific ItemType to load.
    ///
    void setItsTexture(ItemType type);
};

#endif // ITEMVIEW_H
