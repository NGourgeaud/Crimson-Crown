// File Name: collectibleview.h
// Group: A11
// Description: Header file defining the visual rendering component for interactive items like chests.

#ifndef COLLECTIBLEVIEW_H
#define COLLECTIBLEVIEW_H

#include <QPainter>
#include <QPixmap>
#include <QRectF>

class CollectibleView
{
private:
    QPixmap chestTexture;
    QRectF bounds;

public:
    ///
    /// \brief Initializes the view and loads textures.
    ///
    CollectibleView();

    ///
    /// \brief Defines the boundaries for rendering the collectible.
    /// \param x The X coordinate.
    /// \param y The Y coordinate.
    /// \param width The width.
    /// \param height The height.
    ///
    void setBounds(qreal x, qreal y, qreal width, qreal height);

    ///
    /// \brief Renders the collectible, adjusting opacity if opened.
    /// \param painter Reference to the QPainter.
    /// \param isChestClosed Boolean indicating the chest's state.
    ///
    void draw(QPainter &painter, bool isChestClosed);
};

#endif // COLLECTIBLEVIEW_H
