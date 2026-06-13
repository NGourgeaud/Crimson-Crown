// File Name: bossview.cpp
// Group: A11
// Description: View class responsible for rendering boss models, handling texture loading and fallback geometric shapes.

#include "bossview.h"
#include "qpainter.h"

///
/// \brief Default constructor for the BossView.
///
BossView::BossView()
{
}

///
/// \brief Renders the boss on the screen.
/// \details Draws the appropriate texture for the boss. If the texture fails to load, it draws a colored placeholder rectangle based on the boss type.
/// \param painter Pointer to the QPainter object used for drawing operations.
/// \param model Constant pointer to the BossModel containing the data (hitbox, type) to be drawn.
///
void BossView::draw(QPainter * painter, const BossModel * model)
{
    // Fallback drawing if the texture is not loaded properly
    if (itsTexTure.isNull()) {
        painter->setBrush(QBrush(Qt::NoBrush));
        switch (model->getItsType()) {
        case LORD:
            painter->fillRect(model->getHitBox(), QColor(150, 150, 200));
            painter->setPen(Qt::blue);
            painter->drawRect(model->getHitBox());
            break;
        case KING:
            painter->fillRect(model->getHitBox(), QColor(Qt::yellow));
            painter->setPen(Qt::blue);
            painter->drawRect(model->getHitBox());
            break;
        case CLERGY:
            painter->fillRect(model->getHitBox(), Qt::white);
            painter->setPen(Qt::white);
            painter->drawRect(model->getHitBox());
            break;
        default:
            painter->fillRect(model->getHitBox(), Qt::white);
            painter->setPen(Qt::white);
            painter->drawRect(model->getHitBox());
            break;
        }
        return;
    }

    // Draws the actual boss sprite if the texture is valid
    painter->drawPixmap(model->getHitBox().toRect(), itsTexTure);
}

///
/// \brief Loads and assigns the appropriate sprite texture based on the boss type.
/// \param type The BossType indicating which image file to load.
///
void BossView::setItsTexture(BossType type)
{
    switch (type) {
    case LORD:
        itsTexTure = QPixmap(":/lord.png");
        break;
    case KING:
        itsTexTure = QPixmap(":/king.png");
        break;
    case CLERGY:
        itsTexTure = QPixmap(":/clerge.png");
        break;
    default:
        break;
    }
}
