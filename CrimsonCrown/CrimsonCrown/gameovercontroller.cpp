// File Name: gameovercontroller.cpp
// Group: A11
// Description: Controller managing the Game Over screen lifecycle and routing the player back to the main menu.

#include "gameovercontroller.h"

///
/// \brief Initializes the Game Over controller.
/// \param parentWidget The parent widget to host the view.
/// \param parent The parent QObject for memory management.
///
GameOverController::GameOverController(QWidget *parentWidget, QObject *parent)
    : QObject(parent), itsView(nullptr), parentWidget(parentWidget) {}

///
/// \brief Instantiates the Game Over view with the final score and chosen hero, and connects its signals.
/// \param score The final score achieved by the player.
/// \param hero The type of hero the player used during the run.
/// \return A pointer to the newly created GameOverView widget.
///
QWidget* GameOverController::createView(int score, HeroType hero) {
    itsView = new GameOverView(score, hero, parentWidget);

    connect(itsView, &GameOverView::menuRequested, this, &GameOverController::menuRequested);

    return itsView;
}

///
/// \brief Cleanly detaches and frees the Game Over view from memory.
///
void GameOverController::destroyView() {
    if (itsView == nullptr) return;

    disconnect(itsView);
    itsView->deleteLater();
    itsView = nullptr;
}
