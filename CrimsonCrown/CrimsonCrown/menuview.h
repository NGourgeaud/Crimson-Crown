// File Name: menuview.h
// Group: A11
// Description: Header file defining the visual interface for the main menu.

#ifndef MENUVIEW_H
#define MENUVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>

class MenuView : public QWidget {
    Q_OBJECT
public:
    ///
    /// \brief Constructs the main menu view UI.
    /// \param parent Pointer to the parent QWidget container.
    ///
    explicit MenuView(QWidget *parent = nullptr);

signals:
    /// \brief Emitted when the play button is clicked.
    void playRequested();

    /// \brief Emitted when the leaderboard button is clicked.
    void leaderboardRequested();

    /// \brief Emitted when the settings button is clicked.
    void settingsRequested();

    /// \brief Emitted when the quit button is clicked.
    void quitRequested();

protected:
    ///
    /// \brief Draws the main menu background image.
    /// \param event The paint event triggered by Qt.
    ///
    void paintEvent(QPaintEvent *event) override;
};

#endif // MENUVIEW_H
