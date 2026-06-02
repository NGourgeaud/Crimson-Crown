#include "menucontroller.h"

///
/// \brief MenuController::MenuController
/// \details Constructor for the specific Main Menu controller.
/// \param parentWidget Pointer to the container widget (the MainWindow).
/// \param parent Pointer to the parent QObject (the AppController).
///
MenuController::MenuController(QWidget *parentWidget, QObject *parent)
    : QObject(parent), itsView(nullptr), parentWidget(parentWidget) {}

///
/// \brief MenuController::createView
/// \details Instantiates the MenuView and binds its UI signals.
/// Allocates the view and connects its UI interaction signals to this controller
/// \return QWidget* Pointer to the created MenuView, ready to be hosted by the QMainWindow.
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
/// \brief MenuController::destroyView
/// \details Disconnects signals and safely deletes the MenuView from memory.
///
void MenuController::destroyView() {
    if (itsView == nullptr) return;

    disconnect(itsView);
    itsView->deleteLater();
    itsView = nullptr;
}
