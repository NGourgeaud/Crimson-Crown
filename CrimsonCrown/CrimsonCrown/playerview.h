// File Name: playerview.h
// Group: A11
// Description: Header file defining the visual rendering component for the player character.

#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include "playermodel.h"
#include <QWidget>

class PlayerView : public QWidget
{
    Q_OBJECT
private:
    PlayerModel * itsModel;
    QPixmap itsTexture;

public:
    ///
    /// \brief Initializes the player view with its associated model.
    /// \param parent The parent QWidget.
    /// \param aModel Pointer to the PlayerModel.
    ///
    explicit PlayerView(QWidget * parent, PlayerModel * aModel);

    ///
    /// \brief Renders the player model on the screen.
    /// \param painter Pointer to the QPainter.
    ///
    void draw(QPainter *painter);

    ///
    /// \brief Loads the texture corresponding to the hero type.
    /// \param namePlayer The selected HeroType.
    ///
    void setItsTexture(HeroType namePlayer);
};

#endif // PLAYERVIEW_H
