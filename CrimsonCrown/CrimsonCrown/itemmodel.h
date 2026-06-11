#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QObject>
#include "QRectF"
#include "config.h"

class ItemModel : public QObject
{
    Q_OBJECT

private:
    QRectF itsHitbox;
    ItemType itsType;
public:
    explicit ItemModel(QPointF pos, ItemType type, QObject *parent = nullptr);
    ItemType getItsType() const;
    QRectF getItsHitbox() const;

signals:
};

#endif // ITEMMODEL_H
