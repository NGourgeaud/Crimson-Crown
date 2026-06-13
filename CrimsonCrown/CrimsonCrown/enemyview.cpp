// File Name: enemyview.cpp
// Group: A11
// Description: View class responsible for rendering standard enemy models, handling textures and fallback geometric shapes.

#include "enemyview.h"
#include <QtCore/qtpreprocessorsupport.h>

///
/// \brief Default constructor for the EnemyView.
///
EnemyView::EnemyView() {}

///
/// \brief Renders the enemy on the screen.
/// \details Draws the loaded texture, or falls back to drawing colored rectangles based on the enemy type if the texture is missing.
/// \param painter Pointer to the QPainter object used for drawing operations.
/// \param model Pointer to the EnemyModel containing the data (hitbox, type) to be drawn.
///
void EnemyView::draw(QPainter * painter, EnemyModel * model)
{
    // Fallback drawing if the texture is not loaded properly
    if (itsTexTure.isNull()) {
        painter->setBrush(QBrush(Qt::NoBrush));
        switch (model->getitsEnemyType()) {
        case KNIGHT:
            painter->fillRect(model->getHitBox(), QColor(50, 100, 220));
            painter->setPen(Qt::red);
            break;
        case LANCER:
            painter->fillRect(model->getHitBox(), QColor(50, 100, 100));
            painter->setPen(Qt::red);
            break;
        case ARCHER:
            painter->fillRect(model->getHitBox(), QColor(100, 50, 100));
            painter->setPen(Qt::red);
            break;
        default:
            painter->fillRect(model->getHitBox(), QColor(255, 255, 255));
            painter->setPen(Qt::red);
            break;
        }
        painter->drawRect(model->getHitBox());
        return;
    }

    // Draws the actual enemy sprite if the texture is valid
    painter->drawPixmap(model->getHitBox().toRect(), itsTexTure);
}

///
/// \brief Loads and assigns the appropriate sprite texture based on the enemy type.
/// \param type The EnemyType indicating which image file to load.
///
void EnemyView::setItsTexture(EnemyType type)
{
    switch (type) {
    case LANCER:
        itsTexTure = QPixmap(":/lancier.png");
        break;
    case KNIGHT:
        itsTexTure = QPixmap(":/chevalier.png");
        break;
    case ARCHER:
        itsTexTure = QPixmap(":/archer.png");
        break;
    default:
        break;
    }
}
