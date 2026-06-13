// File Name: itemview.cpp
// Group: A11
// Description: View class responsible for rendering collectible items (hearts, gold) on the screen.

#include "itemview.h"

///
/// \brief Default constructor for the ItemView.
///
ItemView::ItemView() {}

///
/// \brief Renders the item on the screen.
/// \details Draws the appropriate texture. If the texture fails to load, it draws a colored fallback rectangle based on the item type.
/// \param painter Pointer to the QPainter object used for drawing operations.
/// \param model Pointer to the ItemModel containing the data (hitbox, type) to be drawn.
///
void ItemView::draw(QPainter * painter, ItemModel * model)
{
    setItsTexture(model->getItsType());

    // Qt best practice: use isNull() to verify if the QPixmap is valid
    if (itsTexture.isNull()){
        switch (model->getItsType()) {
        case HEART:
            painter->setPen(QColor(250, 0, 63));
            painter->setBrush(QColor(250, 0, 63));
            break;
        case GOLD:
            painter->setPen(QColor(239, 191, 4));
            painter->setBrush(QColor(239, 191, 4));
            break;
        default:
            break;
        }
        painter->drawRect(model->getItsHitbox());
        return;
    }

    // Draws the valid texture scaled to the item's hitbox
    painter->drawPixmap(model->getItsHitbox().toRect(), itsTexture);
}

///
/// \brief Loads and assigns the appropriate sprite texture based on the item type.
/// \param type The ItemType indicating which image file to load.
///
void ItemView::setItsTexture(ItemType type)
{
    switch (type) {
    case HEART:
        itsTexture = QPixmap(":/coeur.png");
        break;
    case GOLD:
        itsTexture = QPixmap(":/gold.png");
        break;
    default:
        break;
    }
}
