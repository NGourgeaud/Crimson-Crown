#ifndef PROJECTILEVIEW_H
#define PROJECTILEVIEW_H
#include "config.h"
#include "projectilemodel.h"
#include "qpixmap.h"

class ProjectileView
{
    ProjectileType itsType;
    QPixmap itsTexture;
public:
    ProjectileView();
    void draw(QPainter * painter, ProjectileModel * model);
    void setItsTexture(ProjectileType type);
};

#endif // PROJECTILEVIEW_H
