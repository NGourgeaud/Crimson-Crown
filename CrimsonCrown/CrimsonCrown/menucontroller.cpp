// File Name: menucontroller.cpp
// Group: A11
// Description: Controller handling the main menu logic, routing user actions to the central application orchestrator.

#include "menucontroller.h"

///
/// \brief Initializes the Menu controller and binds the parent widget.
/// \param parentWidget Pointer to the container widget (the MainWindow).
/// \param parent Pointer to the parent QObject (the AppController).
///
MenuController::MenuController(QWidget *parentWidget, QObject *parent)
    : QObject(parent), itsView(nullptr), parentWidget(parentWidget) {}

///
/// \brief Instantiates the MenuView and binds its UI signals.
/// \return Pointer to the created MenuView, ready to be hosted by the QMainWindow.
///
QWidget* MenuController::createView() {
    itsView = new MenuView(parentWidget);

    connect(itsView, &MenuView::playRequested, this, &MenuController::playRequested);
    connect(itsView, &MenuView::leaderboardRequested, this, &MenuController::leaderboardRequested);
    connect(itsView, &MenuView::settingsRequested, this, &MenuController::settingsRequested);
    connect(itsView, &MenuView::quitRequested, this, &MenuController::quitRequested);

    return itsView;
}

///
/// \brief Disconnects signals and safely deletes the MenuView from memory.
///
void MenuController::destroyView() {
    if (itsView == nullptr) return;

    disconnect(itsView);
    itsView->deleteLater();
    itsView = nullptr;
}
