#include "leaderboardcontroller.h"

LeaderboardController::LeaderboardController(QWidget *parentWidget, QObject *parent)
    : QObject(parent), itsModel(new LeaderboardModel()), itsView(nullptr), parentWidget(parentWidget)
{}

LeaderboardController::~LeaderboardController() {
    delete itsModel;
}

QWidget* LeaderboardController::createView() {
    itsView = new LeaderboardView(parentWidget, itsModel->getTopScores());

    connect(itsView, &LeaderboardView::backRequested, this, &LeaderboardController::menuRequested);

    return itsView;
}

void LeaderboardController::destroyView() {
    if (itsView == nullptr) return;
    disconnect(itsView);
    itsView->deleteLater();
    itsView = nullptr;
}

void LeaderboardController::submitScore(const QString& playerName, int score) {
    itsModel->addScore(playerName, score);
}
