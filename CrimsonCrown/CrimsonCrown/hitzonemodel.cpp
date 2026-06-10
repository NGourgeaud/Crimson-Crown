#include "hitzonemodel.h"

AttackZoneType HitZoneModel::getItsType() const
{
    return itsType;
}

QPolygon HitZoneModel::getItsPolygon() const
{
    return itsPolygon;
}

HitZoneModel::HitZoneModel(QPolygon polygon, AttackZoneType type) {
    itsPolygon = polygon;
    itsType = type;
}
