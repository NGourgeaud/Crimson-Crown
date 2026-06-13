// File Name: roomview.cpp
// Group: A11
// Description: View class responsible for rendering dungeon rooms, floor textures, invisible zones, and delegating obstacle drawing.

#include "roomview.h"
#include <QBrush>
#include <QColor>
#include <QPen>

///
/// \brief Initializes the RoomView and loads necessary textures.
///
RoomView::RoomView()
{
    spawnTexture = QPixmap(":/sol-spawn.png");
}

///
/// \brief Renders the room, its background, walls, and delegates obstacle rendering.
/// \param painter Reference to the QPainter object.
/// \param roomModel Pointer to the RoomModel to render.
/// \param obstacleView Reference to the ObstacleView used to draw inner obstacles.
///
void RoomView::draw(QPainter &painter, const RoomModel *roomModel, ObstacleView &obstacleView)
{
    if (!roomModel) return;

    // --- HIDING INVISIBLE ZONES ---
    if (roomModel->getOffsetX() == 2100 && roomModel->getOffsetY() == 225) return;
    if ((roomModel->getOffsetX() == 3475 && roomModel->getOffsetY() == 1625) ||
        (roomModel->getOffsetX() == 3250 && roomModel->getOffsetY() == 2025)) return;

    RoomType type = roomModel->getType();

    // --- DRAWING ROOM BACKGROUND ---
    if (type == RoomType::Spawn) {
        if (!spawnTexture.isNull()) {
            painter.drawPixmap(0, 0, roomModel->getWidth(), roomModel->getHeight(), spawnTexture);
            painter.setBrush(Qt::NoBrush); // Prevents gray from covering the texture
        } else {
            painter.setBrush(QBrush(QColor(64, 64, 64))); // SECURITY: Light gray instead of black
        }
    }
    else if (type == RoomType::Boss) {
        painter.setBrush(QBrush(QColor(80, 20, 20))); // Dark red
    }
    else {
        painter.setBrush(QBrush(QColor(64, 64, 64))); // Classic gray
    }

    // ROOM OUTLINE
    painter.setPen(QPen(Qt::black, 4));
    painter.drawRect(0, 0, roomModel->getWidth(), roomModel->getHeight());

    // DRAWING OBSTACLES
    painter.setPen(QPen(Qt::black, 4));
    for (const ObstacleModel* obstacle : roomModel->getItsObstacles()) {
        obstacleView.draw(painter, obstacle);
    }
}
