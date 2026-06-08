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

    // 1. On récupère la hitbox et l'angle de direction
    QRectF hitbox = model->getItsHitbox();
    qreal angle = model->getItsDiretion().angle();

    // 2. On sauvegarde l'état actuel du pinceau
    painter->save();

    // 3. On déplace le point de pivot au centre exact de la flèche
    painter->translate(hitbox.center());

    // 4. On fait pivoter le pinceau (QLineF donne un angle anti-horaire, donc on inverse avec le signe -)
    painter->rotate(-angle);

    // 5. On dessine l'image en la centrant sur notre nouveau point de pivot (0,0)
    QRectF targetRect(-hitbox.width() / 2.0, -hitbox.height() / 2.0, hitbox.width(), hitbox.height());
    painter->drawPixmap(targetRect.toRect(), itsTexture);

    // 6. On restaure le pinceau à son état normal pour le reste du jeu
    painter->restore();
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
