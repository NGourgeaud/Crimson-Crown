// File Name: projectileview.h
// Group: A11
// Description: Header file defining the visual rendering component for projectiles.

#ifndef PROJECTILEVIEW_H
#define PROJECTILEVIEW_H
#include "config.h"
#include "projectilemodel.h"
#include "qpixmap.h"
#include <QPainter>

class ProjectileView
{
private:
    ProjectileType itsType;
    QPixmap itsTexture;

public:
    ///
    /// \brief Default constructor.
    ///
    ProjectileView();

    ///
    /// \brief Renders the projectile on the screen.
    /// \param painter Pointer to the QPainter.
    /// \param model Pointer to the ProjectileModel.
    ///
    void draw(QPainter * painter, ProjectileModel * model);

    ///
    /// \brief Loads the appropriate texture for the projectile.
    /// \param type The ProjectileType.
    ///
    void setItsTexture(ProjectileType type); };

#endif // PROJECTILEVIEW_H
