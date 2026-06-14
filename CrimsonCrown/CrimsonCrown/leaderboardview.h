// File Name: leaderboardview.h
// Group: A11
// Description: Header file defining the visual interface for the leaderboard screen.

#ifndef LEADERBOARDVIEW_H
#define LEADERBOARDVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "leaderboardmodel.h"

class LeaderboardView : public QWidget {
    Q_OBJECT

private:
    QTableWidget* table;
    QPushButton* backButton;

protected:
    ///
    /// \brief Paints the background image of the leaderboard screen.
    /// \param event The paint event triggered by Qt.
    ///
    void paintEvent(QPaintEvent *event) override;

public:
    ///
    /// \brief Constructs the leaderboard UI with the provided scores.
    /// \param parent The parent QWidget.
    /// \param scores The list of top ScoreEntry objects to display.
    ///
    explicit LeaderboardView(QWidget *parent, const QList<ScoreEntry>& scores);

signals:
    ///
    /// \brief Emitted when the back button is clicked.
    ///
    void backRequested(); };

#endif // LEADERBOARDVIEW_H
