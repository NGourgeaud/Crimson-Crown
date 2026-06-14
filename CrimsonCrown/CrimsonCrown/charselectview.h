// File Name: charselectview.h
// Group: A11
// Description: Header file defining the visual interface for character selection.

#ifndef CHARSELECTVIEW_H
#define CHARSELECTVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include "config.h"

class CharSelectView : public QWidget {
    Q_OBJECT
public:
    ///
    /// \brief Constructs the UI layout for the character selection screen.
    /// \param parent The parent QWidget.
    ///
    explicit CharSelectView(QWidget *parent = nullptr);

    ///
    /// \brief Updates the UI assets to reflect the current hero index.
    ///
    void updateCarousel();

signals:
    ///
    /// \brief Emitted when the user confirms a hero selection.
    /// \param hero The selected HeroType.
    ///
    void selectCharacterRequested(HeroType hero);

    ///
    /// \brief Emitted when the back button is clicked.
    ///
    void backRequested();

protected:
    ///
    /// \brief Draws the background image.
    /// \param event The paint event.
    ///
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *itsTitle;
    QPushButton *itsLeftArrow;
    QPushButton *itsRightArrow;
    QPushButton *itsHeroCardButton;
    QLabel *itsHeroStatsLabel;
    QPushButton *itsBackButton;

    ///
    /// \brief Internal index tracking the current carousel selection.
    ///
    int currentHeroIndex;
};

#endif // CHARSELECTVIEW_H
