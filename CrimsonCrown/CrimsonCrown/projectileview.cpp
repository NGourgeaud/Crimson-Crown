// File Name: projectileview.cpp
// Group: A11
// Description: View class handling the rendering of projectiles, including dynamic rotation based on trajectory.

#include "projectileview.h"
#include "QPainter"

///
/// \brief Default constructor for ProjectileView.
///
ProjectileView::ProjectileView() {}

///
/// \brief Renders the projectile on the screen with the correct rotation and texture.
/// \param painter Pointer to the QPainter object used for drawing.
/// \param model Pointer to the ProjectileModel to render.
///
void ProjectileView::draw(QPainter * painter, ProjectileModel * model)
{
    if (itsTexture.isNull()) {
        if (model->getItsTeam() == PLAYER){
            painter->fillRect(model->getItsHitbox(), QColor(50, 100, 220));
            painter->setPen(QColor(0x964B00));
        }
        else{
            painter->fillRect(model->getItsHitbox(), QColor(200, 50, 50));
            painter->setPen(QColor(0x964B00));
        }
        painter->drawRect(model->getItsHitbox());
        return;
    }

    // 1. Retrieve the hitbox and the direction angle
    QRectF hitbox = model->getItsHitbox();
    qreal angle = model->getItsDiretion().angle();

    // 2. Save the current state of the painter
    painter->save();

    // 3. Move the pivot point to the exact center of the projectile
    painter->translate(hitbox.center());

    // 4. Rotate the painter (QLineF gives a counter-clockwise angle, so we invert it with a minus sign)
    painter->rotate(-angle);

    // 5. Draw the image centered on our new pivot point (0,0)
    QRectF targetRect(-hitbox.width() / 2.0, -hitbox.height() / 2.0, hitbox.width(), hitbox.height());
    painter->drawPixmap(targetRect.toRect(), itsTexture);

    // 6. Restore the painter to its normal state for the rest of the game
    painter->restore();
}

///
/// \brief Loads the appropriate texture based on the projectile type.
/// \param type The ProjectileType indicating which sprite to load.
///
void ProjectileView::setItsTexture(ProjectileType type)
{
    switch (type) {
    case ARROW:
        itsTexture = QPixmap(":/arrow.png");
        break;
    case KNIFE:
        itsTexture = QPixmap(":/knife.png");
        break;
    case RADIUS:
        itsTexture = QPixmap(":/rayon.png");
        break;
    default:
        break;
    }
}
