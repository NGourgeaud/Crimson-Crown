#ifndef HITZONEMODEL_H
#define HITZONEMODEL_H

#include "config.h"
#include "qpolygon.h"
class HitZoneModel
{
private:
    QPolygon itsPolygon;
    AttackZoneType itsType;
public:
    HitZoneModel(QPolygon polygon, AttackZoneType type);
    AttackZoneType getItsType() const;
    QPolygon getItsPolygon() const;

    bool operator==(const HitZoneModel& other) const {
        // Compare les attributs qui rendent une HitZone unique.
        // Par exemple, on peut comparer les polygones :
        return this->getItsPolygon() == other.getItsPolygon();
    }
};

#endif // HITZONEMODEL_H
