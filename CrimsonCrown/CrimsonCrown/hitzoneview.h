// File Name: hitzoneview.h
// Group: A11
// Description: Header file defining the visual rendering for attack hit zones and weapon textures.

#ifndef HITZONEVIEW_H
#define HITZONEVIEW_H

#include "config.h"
#include "hitzonemodel.h"
#include "qpixmap.h"
#include <QPainter>

class HitZoneView
{
private:
    QPixmap itsTexture;
public:
    ///
    /// \brief Default constructor.
    ///
    HitZoneView();

    ///
    /// \brief Renders the hit zone polygon with its appropriate texture or fallback color.
    /// \param painter Pointer to the QPainter.
    /// \param model Pointer to the HitZoneModel to render.
    ///
    void draw(QPainter * painter, HitZoneModel * model);

    ///
    /// \brief Loads the texture corresponding to the attack type.
    /// \param type The specific AttackZoneType.
    ///
    void setItsTexture(AttackZoneType type);
};

#endif // HITZONEVIEW_H
