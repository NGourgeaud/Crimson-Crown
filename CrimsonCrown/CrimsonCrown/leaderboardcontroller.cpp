// File Name: leaderboardcontroller.cpp
// Group: A11
// Description: Controller bridging the leaderboard view and the data model, handling score submissions and navigation.

#include "leaderboardcontroller.h"

///
/// \brief Initializes the Leaderboard controller and its underlying model.
/// \param parentWidget The parent widget to host the view.
/// \param parent The parent QObject for memory management.
///
LeaderboardController::LeaderboardController(QWidget *parentWidget, QObject *parent)
    : QObject(parent), itsModel(new LeaderboardModel()), itsView(nullptr), parentWidget(parentWidget)
{}

///
/// \brief Destructor handling the cleanup of the data model.
///
LeaderboardController::~LeaderboardController() {
    delete itsModel;
}

///
/// \brief Instantiates the Leaderboard view and populates it with the top scores.
/// \return A pointer to the newly created LeaderboardView widget.
///
QWidget* LeaderboardController::createView() {
    itsView = new LeaderboardView(parentWidget, itsModel->getTopScores());

    connect(itsView, &LeaderboardView::backRequested, this, &LeaderboardController::menuRequested);

    return itsView;
}

///
/// \brief Cleanly detaches and frees the Leaderboard view from memory.
///
void LeaderboardController::destroyView() {
    if (itsView == nullptr) return;
    disconnect(itsView);
    itsView->deleteLater();
    itsView = nullptr;
}

///
/// \brief Submits a new player score to the model for sorting and saving.
/// \param playerName The name input by the player.
/// \param score The final score achieved.
///
void LeaderboardController::submitScore(const QString& playerName, int score) {
    itsModel->addScore(playerName, score);
}
