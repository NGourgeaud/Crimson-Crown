#include "playerview.h"

PlayerView::PlayerView(QWidget *parent, PlayerModel * model)
    : QWidget{parent}, itsModel(model)
{}


void PlayerView::draw(QPainter * painter)
{
    // Regarder dans menu view et aide dans mario mvc
}
