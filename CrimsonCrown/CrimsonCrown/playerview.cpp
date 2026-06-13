// File Name: playerview.cpp
// Group: A11
// Description: View class responsible for rendering the player character, handling textures and specific visual states like invisibility.

#include "playerview.h"
#include "assassinmodel.h"
#include <QPainter>

///
/// \brief Initializes the player view with the associated model.
/// \param parent The parent QWidget.
/// \param aModel Pointer to the PlayerModel to render.
///
PlayerView::PlayerView(QWidget * parent, PlayerModel * aModel)
    : QWidget{parent}, itsModel(aModel)
{}

///
/// \brief Renders the player on the screen, handling specific visual states.
/// \param painter Pointer to the QPainter used for drawing.
///
void PlayerView::draw(QPainter * painter)
{
    bool isInvisible = false;

    AssassinModel* assassin = dynamic_cast<AssassinModel*>(itsModel);
    if (assassin && assassin->getIsInvisible()) {
        isInvisible = true;
        // Makes the player semi-transparent (0.4 = 40% opacity)
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

///
/// \brief Loads and sets the correct texture based on the chosen hero type.
/// \param namePlayer The HeroType indicating which sprite to load.
///
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
