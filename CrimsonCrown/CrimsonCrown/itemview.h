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
    ItemView();
    void draw(QPainter * painter, ItemModel * model);
    void setItsTexture(ItemType type);
};

#endif // ITEMVIEW_H
