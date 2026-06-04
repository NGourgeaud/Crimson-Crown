#include "bossview.h"
#include "qpainter.h"

BossView::BossView(BossType type, const BossModel * model) : itsModel(model)
{
    itsType = type;
}

void BossView::draw(QPainter * painter)
{
    if (itsTexTure.isNull()) {
        painter->fillRect(itsModel->getHitBox(), QColor(200, 50, 50));
        painter->setPen(Qt::red);
        painter->drawRect(itsModel->getHitBox());
        return;
    }

    painter->drawPixmap(itsModel->getHitBox().topLeft(), itsTexTure);
}

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
