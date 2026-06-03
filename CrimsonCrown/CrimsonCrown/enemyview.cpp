#include "enemyview.h"


EnemyView::EnemyView(EnemyType type, const EnemyModel *model) : itsModel(model)
{
    itsType = type;
}

void EnemyView::draw(QPainter * painter)
{
    if (itsTexTure.isNull()) {
        painter->fillRect(itsModel->getHitBox(), QColor(50, 100, 220));
        painter->setPen(Qt::red);
        painter->drawRect(itsModel->getHitBox());
        return;
    }

    painter->drawPixmap(itsModel->getHitBox().topLeft(), itsTexTure);
}

void EnemyView::setItsTexture(EnemyType type)
{
    switch (itsType) {
    case LANCER:
        itsTexTure = QPixmap(":/lancier.png");
        break;
    default:
        break;
    }
}
