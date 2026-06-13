// File Name: leaderboardcontroller.h
// Group: A11
// Description: Header file defining the controller for the leaderboard screen.

#ifndef LEADERBOARDCONTROLLER_H
#define LEADERBOARDCONTROLLER_H

#include <QObject>
#include <QWidget>
#include "leaderboardmodel.h"
#include "leaderboardview.h"

class LeaderboardController : public QObject {
    Q_OBJECT
private:
    LeaderboardModel* itsModel;
    LeaderboardView* itsView;
    QWidget* parentWidget;

public:
    ///
    /// \brief Initializes the leaderboard controller.
    /// \param parentWidget The parent QWidget.
    /// \param parent The parent QObject.
    ///
    explicit LeaderboardController(QWidget *parentWidget, QObject *parent = nullptr);

    ///
    /// \brief Destructor handling the cleanup of the data model.
    ///
    ~LeaderboardController();

    ///
    /// \brief Creates and populates the leaderboard view.
    /// \return Pointer to the generated QWidget.
    ///
    QWidget* createView();

    ///
    /// \brief Destroys the view and frees memory.
    ///
    void destroyView();

    ///
    /// \brief Submits a new player score to the model.
    /// \param playerName The name input by the player.
    /// \param score The final score achieved.
    ///
    void submitScore(const QString& playerName, int score);

signals:
    ///
    /// \brief Emitted when the user requests to return to the main menu.
    ///
    void menuRequested(); };

#endif // LEADERBOARDCONTROLLER_H
