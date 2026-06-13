// File Name: hitzonemodel.cpp
// Group: A11
// Description: Data model representing a physical attack area (hitbox polygon) and its specific weapon type.

#include "hitzonemodel.h"

///
/// \brief Retrieves the weapon or attack type associated with this hit zone.
/// \return The AttackZoneType enum value.
///
AttackZoneType HitZoneModel::getItsType() const
{
    return itsType;
}

///
/// \brief Retrieves the geometric shape defining the attack's physical boundaries.
/// \return A QPolygon representing the hit area.
///
QPolygon HitZoneModel::getItsPolygon() const
{
    return itsPolygon;
}

///
/// \brief Constructs a hit zone model with a specific shape and type.
/// \param polygon The geometric shape of the attack area.
/// \param type The specific type of the attack (e.g., SWORD, HAMMER).
///
HitZoneModel::HitZoneModel(QPolygon polygon, AttackZoneType type) {
    itsPolygon = polygon;
    itsType = type;
}
