// File Name: obstacleview.cpp
// Group: A11
// Description: View class responsible for rendering static obstacles onto the game screen.

#include "obstacleview.h"
#include <QBrush>
#include <QColor>

///
/// \brief Default constructor for the ObstacleView.
///
ObstacleView::ObstacleView()
{
}

///
/// \brief Renders the obstacle on the screen using its defined hitbox.
/// \param painter Reference to the QPainter object used for drawing.
/// \param model Pointer to the ObstacleModel containing the hitbox data.
///
void ObstacleView::draw(QPainter &painter, const ObstacleModel *model)
{
    QRect rect = model->getHitBox();

    // Draws the obstacle with a solid wood color
    painter.setBrush(QBrush(QColor(139, 69, 19)));
    painter.drawRect(rect);
}
