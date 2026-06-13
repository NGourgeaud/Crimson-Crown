// File Name: hitzoneview.cpp
// Group: A11
// Description: View class responsible for rendering attack hit zones with specific textures or fallback debug colors.

#include "hitzoneview.h"
#include "qpainter.h"

///
/// \brief Default constructor for the hit zone view.
///
HitZoneView::HitZoneView() {}

///
/// \brief Renders the attack hit zone on the screen using its assigned texture or fallback colors.
/// \param painter Pointer to the QPainter object used for drawing.
/// \param model Pointer to the HitZoneModel containing the polygon and type data.
///
void HitZoneView::draw(QPainter * painter, HitZoneModel * model)
{
    setItsTexture(model->getItsType());

    // Fallback rendering if the texture is not found
    if (itsTexture.isNull()) {
        switch (model->getItsType()) {
        case UNDEFINEDE:
        case LANCE:
        case SWORD:
            painter->setPen(Qt::red);
            painter->setBrush(QBrush(Qt::white));
            painter->drawPolygon(model->getItsPolygon());
            break;
        case UNDEFINEDP:
        case HAMMER:
            painter->setPen(Qt::blue);
            painter->setBrush(QBrush(Qt::white));
            painter->drawPolygon(model->getItsPolygon());
            break;
        default:
            painter->setPen(Qt::black);
            painter->setBrush(QBrush(Qt::yellow));
            painter->drawPolygon(model->getItsPolygon());
            break;
        }
        return;
    }

    // Standard texture rendering
    QBrush textureBrush(itsTexture);
    painter->setBrush(textureBrush);
    painter->setPen(Qt::NoPen);
    painter->drawPolygon(model->getItsPolygon());
}

///
/// \brief Loads the corresponding visual texture based on the attack type.
/// \param type The AttackZoneType indicating which weapon image to load.
///
void HitZoneView::setItsTexture(AttackZoneType type)
{
    switch (type) {
    case HAMMER:
        itsTexture = QPixmap(":/hammer.png");
        break;
    case LANCE:
        itsTexture = QPixmap(":/lance.png");
        break;
    case SWORD:
        itsTexture = QPixmap(":/sword.png");
        break;
    default:
        break;
    }
}
