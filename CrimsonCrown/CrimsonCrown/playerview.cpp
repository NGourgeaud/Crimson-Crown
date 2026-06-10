#include "playerview.h"
#include "assassinmodel.h"
#include <QPainter>

PlayerView::PlayerView(QWidget * parent, PlayerModel * aModel)
    : QWidget{parent}, itsModel(aModel)
{}


void PlayerView::draw(QPainter * painter)
{
    bool isInvisible = false;

    AssassinModel* assassin = dynamic_cast<AssassinModel*>(itsModel);
    if (assassin && assassin->getIsInvisible()) {
        isInvisible = true;
        // On rend le joueur semi-transparent (0.4 = 40% d'opacité)
        painter->setOpacity(0.4);
    }

    if (itsTexture.isNull()) {
        painter->fillRect(itsModel->getHitBox(), QColor(50, 100, 220));
        painter->setPen(Qt::white);
        painter->drawRect(itsModel->getHitBox());
        return;
    }

    painter->drawPixmap(itsModel->getHitBox().toRect(), itsTexture);

    if (isInvisible) {
        painter->setOpacity(1.0);
    }

}

void PlayerView::setItsTexture(HeroType namePlayer)
{
    switch (namePlayer) {
    case HeroType::Blacksmith:
        itsTexture = QPixmap(":/forgeron.png");
        break;
    case HeroType::Assassin:
        itsTexture = QPixmap(":/assassin.png");
        break;
    case HeroType::Hunter:
        itsTexture = QPixmap(":/chasseur.png");
        break;
    default:
        break;
    }
}

