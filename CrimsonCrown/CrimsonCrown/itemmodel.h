#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QObject>
#include "QRectF"
#include "config.h"

class ItemModel
{

private:
    QRectF itsHitbox;
    ItemType itsType;
public:
    explicit ItemModel(QPointF pos, ItemType type);
    ItemType getItsType() const;
    QRectF getItsHitbox() const;

};

#endif // ITEMMODEL_H
