#include "enemyview.h"
#include <QtCore/qtpreprocessorsupport.h>


EnemyView::EnemyView() {}

///
/// \brief EnemyView::draw
/// \param painter
///
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

///
/// \brief EnemyView::setItsTexture
/// \param type
///
void EnemyView::setItsTexture(EnemyType type)
{
    switch (type) {
    case LANCER:
        itsTexTure = QPixmap(":/lancier.png");
        break;
    default:
        break;
    }
}
