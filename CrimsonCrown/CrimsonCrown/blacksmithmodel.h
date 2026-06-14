// File Name: blacksmithmodel.h
// Group: A11
// Description: Header file defining the Blacksmith hero character and its abilities.

#ifndef BLACKSMITHMODEL_H
#define BLACKSMITHMODEL_H

#include "hitzonemodel.h"
#include "playermodel.h"
#include <QPolygon>
#include <QLineF>
#include <QTransform>
#include <QTimer>

class BlacksmithModel : public PlayerModel
{
    Q_OBJECT
public:
    ///
    /// \brief Initializes the Blacksmith player character.
    ///
    BlacksmithModel();

    ///
    /// \brief Executes the primary directional hammer swing attack.
    ///
    void attack1();

    ///
    /// \brief Executes the secondary circular area-of-effect attack.
    ///
    void attack2();

signals:
    ///
    /// \brief Emitted when an attack hit zone is generated.
    /// \param attackHitBox The shape and weapon type of the attack.
    /// \param damage The damage to be inflicted.
    ///
    void attackPlayer(HitZoneModel attackHitBox, int damage);
};

#endif // BLACKSMITHMODEL_H
