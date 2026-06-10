#include "hitzoneview.h"
#include "qpainter.h"

HitZoneView::HitZoneView() {}

void HitZoneView::draw(QPainter * painter, HitZoneModel * model)
{
    setItsTexture(model->getItsType());
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

    QBrush textureBrush(itsTexture);
    painter->setBrush(textureBrush);
    painter->setPen(Qt::NoPen);
    painter->drawPolygon(model->getItsPolygon());
}

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
