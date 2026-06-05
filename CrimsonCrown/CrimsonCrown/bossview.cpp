#include "bossview.h"
#include "qpainter.h"

BossView::BossView(BossType type)
{
    itsType = type;
}

///
/// \brief BossView::draw
/// \param painter
/// \param model
///
void BossView::draw(QPainter * painter, const BossModel * model)
{
    if (itsTexTure.isNull()) {
        painter->fillRect(model->getHitBox(), QColor(0, 0, 200));
        painter->setPen(Qt::blue);
        painter->drawRect(model->getHitBox());
        return;
    }

    painter->drawPixmap(model->getHitBox().topLeft(), itsTexTure);
}

///
/// \brief BossView::setItsTexture
///
void BossView::setItsTexture()
{
    switch (itsType) {
    case LORD:
        itsTexTure = QPixmap(":/lord.png");
        break;
    default:
        break;
    }
}
