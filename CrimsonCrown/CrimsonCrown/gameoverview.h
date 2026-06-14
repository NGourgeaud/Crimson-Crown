// File Name: gameoverview.h
// Group: A11
// Description: Header file defining the visual interface for the game over screen.

#ifndef GAMEOVERVIEW_H
#define GAMEOVERVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include "config.h"

class GameOverView : public QWidget {
    Q_OBJECT
public:
    ///
    /// \brief Constructs the game over screen UI.
    /// \param score The final score of the player.
    /// \param hero The hero type used to display the corresponding defeat sprite.
    /// \param parent The parent QWidget.
    ///
    explicit GameOverView(int score, HeroType hero, QWidget *parent = nullptr);

signals:
    ///
    /// \brief Emitted when the user requests to return to the main menu.
    ///
    void menuRequested();

protected:
    ///
    /// \brief Paints the background image of the game over screen.
    /// \param event The paint event triggered by Qt.
    ///
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *itsScoreLabel;
    QPushButton *itsBackButton;
};

#endif // GAMEOVERVIEW_H
