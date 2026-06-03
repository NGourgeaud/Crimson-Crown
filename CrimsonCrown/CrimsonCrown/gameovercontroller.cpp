#include "gameovercontroller.h"

GameOverController::GameOverController(QWidget *parentWidget, QObject *parent)
    : QObject(parent), itsView(nullptr), parentWidget(parentWidget) {}

QWidget* GameOverController::createView(int score) {
    itsView = new GameOverView(score, parentWidget);

    connect(itsView, &GameOverView::menuRequested, this, &GameOverController::backToMenuRequested);

    return itsView;
}

void GameOverController::destroyView() {
    if (itsView == nullptr) return;

    disconnect(itsView);
    itsView->deleteLater();
    itsView = nullptr;
}
