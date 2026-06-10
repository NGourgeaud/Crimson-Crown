#include "gameovercontroller.h"

GameOverController::GameOverController(QWidget *parentWidget, QObject *parent)
    : QObject(parent), itsView(nullptr), parentWidget(parentWidget) {}

QWidget* GameOverController::createView(int score, HeroType hero) {
    itsView = new GameOverView(score, hero, parentWidget);

    connect(itsView, &GameOverView::menuRequested, this, &GameOverController::menuRequested);

    return itsView;
}

void GameOverController::destroyView() {
    if (itsView == nullptr) return;

    disconnect(itsView);
    itsView->deleteLater();
    itsView = nullptr;
}
