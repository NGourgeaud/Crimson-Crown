#include "charselectcontroller.h"

///
/// \brief CharSelectController::CharSelectController
///
CharSelectController::CharSelectController(QWidget *parentWidget, QObject *parent)
    : QObject(parent), itsView(nullptr), parentWidget(parentWidget) {}

///
/// \brief CharSelectController::createView
/// \details Instantiates the view context and forwards inner signals up to AppController.
///
QWidget* CharSelectController::createView() {
    itsView = new CharSelectView(parentWidget);

    connect(itsView, &CharSelectView::selectCharacterRequested, this, &CharSelectController::characterSelected);
    connect(itsView, &CharSelectView::backRequested, this, &CharSelectController::backToMenuRequested);

    return itsView;
}

///
/// \brief CharSelectController::destroyView
/// \details Cleanly detaches and frees the view from the event loop.
///
void CharSelectController::destroyView() {
    if (itsView == nullptr) return;

    disconnect(itsView);
    itsView->deleteLater();
    itsView = nullptr;
}
