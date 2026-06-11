#include "itemview.h"

ItemView::ItemView() {}

void ItemView::draw(QPainter * painter, ItemModel * model)
{
    setItsTexture(model->getItsType());
    if (!itsTexture){
        switch (model->getItsType()) {
        case HEART:
            painter->setPen(QColor(250,0,63));
            painter->setBrush(QColor(250,0,63));
            break;
        case GOLD:
            painter->setPen(QColor(239,191,4));
            painter->setBrush(QColor(239,191,4));
            break;
        default:
            break;
        }
        painter->drawRect(model->getItsHitbox());
        return;
    }

    painter->drawPixmap(model->getItsHitbox().toRect(), itsTexture);
}

void ItemView::setItsTexture(ItemType type)
{
    switch (type) {
    case HEART:
        itsTexture = QPixmap(":/coeur.png");
        break;
    case GOLD:
        itsTexture = QPixmap(":/gold.png");
        break;
    default:
        break;
    }
}
