// File Name: gamewoncontroller.cpp
// Group: A11
// Description: Controller handling the victory screen lifecycle and routing the player back to the main menu.

#include "gamewoncontroller.h"

///
/// \brief Initializes the Game Won controller.
/// \param parentWidget The parent widget to host the view.
/// \param parent The parent QObject for memory management.
///
GameWonController::GameWonController(QWidget *parentWidget, QObject *parent)
    : QObject(parent), itsView(nullptr), parentWidget(parentWidget) {}

///
/// \brief Instantiates the Game Won view with the final score and chosen hero, and connects its signals.
/// \param score The final score achieved by the player.
/// \param hero The type of hero the player used to win the game.
/// \return A pointer to the newly created GameWonView widget.
///
QWidget* GameWonController::createView(int score, HeroType hero) {
    itsView = new GameWonView(score, hero, parentWidget);

    connect(itsView, &GameWonView::menuRequested, this, &GameWonController::menuRequested);

    return itsView;
}

///
/// \brief Cleanly detaches and frees the Game Won view from memory.
///
void GameWonController::destroyView() {
    if (itsView == nullptr) return;

    disconnect(itsView);
    itsView->deleteLater();
    itsView = nullptr;
}
