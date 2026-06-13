// File Name: gamewoncontroller.h
// Group: A11
// Description: Header file defining the controller for the victory screen.

#ifndef GAMEWONCONTROLLER_H
#define GAMEWONCONTROLLER_H

#include <QObject>
#include "gamewonview.h"

class GameWonController : public QObject {
    Q_OBJECT
public:
    ///
    /// \brief Initializes the victory screen controller.
    /// \param parentWidget The parent QWidget.
    /// \param parent The parent QObject.
    ///
    explicit GameWonController(QWidget *parentWidget = nullptr, QObject *parent = nullptr);

    ///
    /// \brief Instantiates the victory view.
    /// \param score The final score to display.
    /// \param hero The type of hero to render the correct victory sprite.
    /// \return A pointer to the newly created view.
    ///
    QWidget* createView(int score, HeroType hero);

    ///
    /// \brief Cleanly detaches and frees the view from memory.
    ///
    void destroyView();

signals:
    ///
    /// \brief Emitted to request navigation back to the main menu.
    ///
    void menuRequested();

private:
    GameWonView *itsView;
    QWidget *parentWidget;
};

#endif // GAMEWONCONTROLLER_H
