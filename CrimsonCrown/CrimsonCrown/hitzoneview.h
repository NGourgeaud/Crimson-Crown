#ifndef HITZONEVIEW_H
#define HITZONEVIEW_H


#include "config.h"
#include "hitzonemodel.h"
#include "qpixmap.h"
class HitZoneView
{
private:
    QPixmap itsTexture;
public:
    HitZoneView();
    void draw(QPainter * painter, HitZoneModel * model);
    void setItsTexture(AttackZoneType type);
};

#endif // HITZONEVIEW_H
