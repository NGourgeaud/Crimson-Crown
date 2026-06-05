#include "gamewoncontroller.h"

GameWonController::GameWonController(QWidget *parentWidget, QObject *parent)
    : QObject(parent), itsView(nullptr), parentWidget(parentWidget) {}

QWidget* GameWonController::createView(int score) {
    itsView = new GameWonView(score, parentWidget);

    connect(itsView, &GameWonView::menuRequested, this, &GameWonController::menuRequested);

    return itsView;
}

void GameWonController::destroyView() {
    if (itsView == nullptr) return;

    disconnect(itsView);
    itsView->deleteLater();
    itsView = nullptr;
}
