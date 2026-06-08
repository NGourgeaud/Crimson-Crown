#include "projectileview.h"
#include "QPainter"

ProjectileView::ProjectileView() {}

///
/// \brief EnemyView::draw
/// \param painter
///
void ProjectileView::draw(QPainter * painter, ProjectileModel * model)
{
    if (itsTexture.isNull()) {
        if (model->getItsTeam() == PLAYER){
        painter->fillRect(model->getItsHitbox(), QColor(50, 100, 220));
        painter->setPen(QColor(0x964B00));
        }
        else{
            painter->fillRect(model->getItsHitbox(), QColor(200, 50, 50));
            painter->setPen(QColor(0x964B00));
        }
        painter->drawRect(model->getItsHitbox());
        return;
    }

    painter->drawPixmap(model->getItsHitbox().topLeft(), itsTexture);
}

///
/// \brief EnemyView::setItsTexture
/// \param type
///
void ProjectileView::setItsTexture(ProjectileType type)
{
    switch (type) {
    case ARROW:
        itsTexture = QPixmap(":/arrow.png");
        break;
    default:
        break;
    }
}
