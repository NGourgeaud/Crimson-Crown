// File Name: collectibleview.cpp
// Group: A11
// Description: View class for collectibles (like chests), handling their rendering and visual states (open/closed).

#include "collectibleview.h"

///
/// \brief Initializes the view and loads the collectible's texture.
///
CollectibleView::CollectibleView()
{
    // Load the Easter egg chest texture from the resource file
    chestTexture = QPixmap(":/coffre-easterEgg.png");
}

///
/// \brief Defines the physical boundaries for drawing the collectible.
/// \param x The X coordinate of the top-left corner.
/// \param y The Y coordinate of the top-left corner.
/// \param width The width of the bounding box.
/// \param height The height of the bounding box.
///
void CollectibleView::setBounds(qreal x, qreal y, qreal width, qreal height)
{
    bounds = QRectF(x, y, width, height);
}

///
/// \brief Renders the collectible on the screen.
/// \details Applies transparency if the chest is already opened.
/// \param painter The QPainter reference used for drawing operations.
/// \param isChestClosed Boolean indicating if the chest is in a closed state.
///
void CollectibleView::draw(QPainter &painter, bool isChestClosed)
{
    if (chestTexture.isNull()) return;

    // Apply opacity if the chest is already opened to visually distinguish it
    if (!isChestClosed) {
        painter.save();
        painter.setOpacity(0.4); // 40% opacity
    }

    // Draw the texture scaled to fit the entire defined bounding box
    painter.drawPixmap(bounds.toRect(), chestTexture);

    if (!isChestClosed) {
        painter.restore();
    }
}
