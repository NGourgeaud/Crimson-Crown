// File Name: gameovercontroller.h
// Group: A11
// Description: Header file for the Game Over screen controller.

#ifndef GAMEOVERCONTROLLER_H
#define GAMEOVERCONTROLLER_H

#include <QObject>
#include "gameoverview.h"

class GameOverController : public QObject {
    Q_OBJECT
public:
    ///
    /// \brief Initializes the Game Over controller.
    /// \param parentWidget The parent widget to host the view.
    /// \param parent The parent QObject.
    ///
    explicit GameOverController(QWidget *parentWidget = nullptr, QObject *parent = nullptr);

    ///
    /// \brief Instantiates the Game Over view.
    /// \param score The final score to display.
    /// \param hero The type of hero to render the correct defeated sprite.
    /// \return A pointer to the newly created view.
    ///
    QWidget* createView(int score, HeroType hero);

    ///
    /// \brief Cleanly detaches and frees the Game Over view from memory.
    ///
    void destroyView();

signals:
    ///
    /// \brief Emitted to request navigation back to the main menu.
    ///
    void menuRequested();

private:
    GameOverView *itsView;
    QWidget *parentWidget;
};

#endif // GAMEOVERCONTROLLER_H
