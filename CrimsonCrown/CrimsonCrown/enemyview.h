// File Name: enemyview.h
// Group: A11
// Description: Header file defining the visual rendering component for standard enemies.

#ifndef ENEMYVIEW_H
#define ENEMYVIEW_H

#include "enemymodel.h"
#include "qpainter.h"
#include "config.h"

class EnemyView
{
private:
    QPixmap itsTexTure;

public:
    ///
    /// \brief Default constructor.
    ///
    EnemyView();

    ///
    /// \brief Renders the enemy on the screen.
    /// \param painter Pointer to the QPainter object used for drawing.
    /// \param model Pointer to the EnemyModel containing the data to be drawn.
    ///
    void draw(QPainter * painter, EnemyModel * model);

    ///
    /// \brief Assigns the correct texture based on the enemy type.
    /// \param type The specific EnemyType to load.
    ///
    void setItsTexture(EnemyType type);
};

#endif // ENEMYVIEW_H
