#include "itemmodel.h"

ItemType ItemModel::getItsType() const
{
    return itsType;
}

QRectF ItemModel::getItsHitbox() const
{
    return itsHitbox;
}

ItemModel::ItemModel(QPointF pos, ItemType type)
{
    itsHitbox = QRectF(pos, QSizeF(DropSize,DropSize));
    itsType = type;
}
