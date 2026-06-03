#include "playerview.h"
#include <QPainter>

PlayerView::PlayerView(QWidget * parent, PlayerModel * aModel)
    : QWidget{parent}, itsModel(aModel)
{}


void PlayerView::draw(QPainter * painter)
{
    if (itsTexture.isNull()) {
        painter->fillRect(itsModel->getHitBox(), QColor(50, 100, 220));
        painter->setPen(Qt::white);
        painter->drawRect(itsModel->getHitBox());
        return;
    }

    painter->drawPixmap(itsModel->getHitBox().topLeft(), itsTexture);
}

void PlayerView::setItsTexture(HeroType namePlayer)
{
    switch (namePlayer) {
    case HeroType::Blacksmith:
        itsTexture = QPixmap(":/forgeron.png");
        break;
    default:
        break;
    }
}
