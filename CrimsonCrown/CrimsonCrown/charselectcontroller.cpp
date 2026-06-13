// File Name: charselectcontroller.cpp
// Group: A11
// Description: Controller class bridging the character selection view and the main application, managing view lifecycle and signal forwarding.

#include "charselectcontroller.h"

///
/// \brief Initializes the character selection controller.
/// \param parentWidget The parent QWidget used to host the view.
/// \param parent The parent QObject for memory management.
///
CharSelectController::CharSelectController(QWidget *parentWidget, QObject *parent)
    : QObject(parent), itsView(nullptr), parentWidget(parentWidget) {}

///
/// \brief Instantiates the view context and forwards inner signals up to AppController.
/// \return A pointer to the newly created CharSelectView widget.
///
QWidget* CharSelectController::createView() {
    itsView = new CharSelectView(parentWidget);

    connect(itsView, &CharSelectView::selectCharacterRequested, this, &CharSelectController::characterSelected);
    connect(itsView, &CharSelectView::backRequested, this, &CharSelectController::backToMenuRequested);

    return itsView;
}

///
/// \brief Cleanly detaches and frees the view from the event loop to prevent memory leaks.
///
void CharSelectController::destroyView() {
    if (itsView == nullptr) return;

    disconnect(itsView);
    itsView->deleteLater();
    itsView = nullptr;
}
