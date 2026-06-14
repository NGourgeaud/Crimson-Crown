// File Name: bossview.h
// Group: A11
// Description: Header file defining the visual rendering component for boss entities.

#ifndef BOSSVIEW_H
#define BOSSVIEW_H

#include "config.h"
#include "bossmodel.h"
#include "qpixmap.h"

class BossView
{
private:
    QPixmap itsTexTure;

public:
    ///
    /// \brief Default constructor.
    ///
    BossView();

    ///
    /// \brief Renders the boss model onto the screen.
    /// \param painter Pointer to the QPainter object.
    /// \param model Pointer to the data model containing position and type.
    ///
    void draw(QPainter * painter, const BossModel *model);

    ///
    /// \brief Assigns the correct texture based on the boss type.
    /// \param type The specific BossType to load.
    ///
    void setItsTexture(BossType type);
};

#endif // BOSSVIEW_H
