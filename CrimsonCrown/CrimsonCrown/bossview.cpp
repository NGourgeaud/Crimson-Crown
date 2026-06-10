#include "bossview.h"
#include "qpainter.h"

BossView::BossView()
{
}

///
/// \brief BossView::draw
/// \param painter
/// \param model
///
void BossView::draw(QPainter * painter, const BossModel * model)
{
    if (itsTexTure.isNull()) {
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
        default:
            painter->fillRect(model->getHitBox(), Qt::white);
            painter->setPen(Qt::white);
            painter->drawRect(model->getHitBox());
            break;
        }
        return;
    }

    painter->drawPixmap(model->getHitBox().toRect(), itsTexTure);
}

///
/// \brief BossView::setItsTexture
///
void BossView::setItsTexture(BossType type)
{
    switch (type) {
    case LORD:
        itsTexTure = QPixmap(":/lord.png");
        break;
    case KING:
        itsTexTure = QPixmap(":/king.png");
    default:
        break;
    }
}
