// File Name: charselectcontroller.h
// Group: A11
// Description: Header file defining the controller for the character selection screen.

#ifndef CHARSELECTCONTROLLER_H
#define CHARSELECTCONTROLLER_H

#include <QObject>
#include "charselectview.h"

class CharSelectController : public QObject {
    Q_OBJECT
public:
    ///
    /// \brief Initializes the character selection controller.
    /// \param parentWidget The parent QWidget.
    /// \param parent The parent QObject.
    ///
    explicit CharSelectController(QWidget *parentWidget = nullptr, QObject *parent = nullptr);

    ///
    /// \brief Creates and binds the character selection view.
    /// \return Pointer to the generated QWidget.
    ///
    QWidget *createView();

    ///
    /// \brief Destroys the view and frees memory.
    ///
    void destroyView();

signals:
    ///
    /// \brief Emitted when a hero is locked in by the player.
    /// \param type The HeroType chosen.
    ///
    void characterSelected(HeroType type);

    ///
    /// \brief Emitted when the user wants to return to the main menu.
    ///
    void backToMenuRequested();

private:
    CharSelectView *itsView;
    QWidget *parentWidget;

};

#endif // CHARSELECTCONTROLLER_H
