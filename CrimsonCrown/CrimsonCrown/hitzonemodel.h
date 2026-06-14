// File Name: hitzonemodel.h
// Group: A11
// Description: Header file defining the data model for attack hit zones and their weapon types.

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
    ///
    /// \brief Constructs a hit zone with a specific geometry and type.
    /// \param polygon The shape of the hit zone.
    /// \param type The weapon or attack type.
    ///
    HitZoneModel(QPolygon polygon, AttackZoneType type);

    ///
    /// \brief Retrieves the type of the attack zone.
    /// \return The AttackZoneType enumerator.
    ///
    AttackZoneType getItsType() const;

    ///
    /// \brief Retrieves the geometric shape of the hit zone.
    /// \return A QPolygon defining the area.
    ///
    QPolygon getItsPolygon() const;

    ///
    /// \brief Overloads the equality operator to compare hit zones.
    /// \param other The other HitZoneModel to compare to.
    /// \return True if the polygons match, false otherwise.
    /// \details Compares attributes that make a HitZone unique. For example, comparing the polygons.
    ///
    bool operator==(const HitZoneModel& other) const {
        return this->getItsPolygon() == other.getItsPolygon();
    }
};

#endif // HITZONEMODEL_H
