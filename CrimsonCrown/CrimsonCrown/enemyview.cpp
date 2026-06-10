#include "enemyview.h"
#include <QtCore/qtpreprocessorsupport.h>


EnemyView::EnemyView() {}

///
/// \brief EnemyView::draw
/// \param painter
///
void EnemyView::draw(QPainter * painter, EnemyModel * model)
{
    if (itsTexTure.isNull()) {
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

    painter->drawPixmap(model->getHitBox().toRect(), itsTexTure);
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
