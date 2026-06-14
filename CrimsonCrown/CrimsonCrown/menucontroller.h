// File Name: menucontroller.h
// Group: A11
// Description: Header file defining the controller for the main menu screen.

#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include <QObject>
#include "menuview.h"

class MenuController : public QObject {
    Q_OBJECT
public:
    ///
    /// \brief Initializes the Menu controller.
    /// \param parentWidget Pointer to the container widget.
    /// \param parent Pointer to the parent QObject.
    ///
    explicit MenuController(QWidget *parentWidget = nullptr, QObject *parent = nullptr);

    ///
    /// \brief Instantiates the menu view and binds its signals.
    /// \return Pointer to the created view.
    ///
    QWidget *createView();

    ///
    /// \brief Destroys the view and frees memory.
    ///
    void destroyView();

signals:
    /// \brief Emitted when the user wants to start the game.
    void playRequested();

    /// \brief Emitted when the user accesses the leaderboard.
    void leaderboardRequested();

    /// \brief Emitted when the user accesses the settings.
    void settingsRequested();

    /// \brief Emitted when the user wants to quit the game.
    void quitRequested();

private:
    MenuView *itsView;
    QWidget *parentWidget;
};

#endif // MENUCONTROLLER_H
