// File Name: leaderboardmodel.h
// Group: A11
// Description: Header file defining the data model handling leaderboard scores storage and retrieval.

#ifndef LEADERBOARDMODEL_H
#define LEADERBOARDMODEL_H

#include <QString>
#include <QList>

///
/// \brief Structure representing a single leaderboard entry.
///
struct ScoreEntry {
    QString playerName;
    int score;
};

class LeaderboardModel {
private:
    QList<ScoreEntry> topScores;
    const QString filePath = "scores.txt";

public:
    ///
    /// \brief Constructs the leaderboard model and loads scores from disk.
    ///
    LeaderboardModel();

    ///
    /// \brief Adds a new score, sorts the list, and saves it.
    /// \param name The name of the player.
    /// \param score The integer score achieved.
    ///
    void addScore(const QString& name, int score);

    ///
    /// \brief Retrieves the sorted list of top scores.
    /// \return A QList of ScoreEntry structures.
    ///
    QList<ScoreEntry> getTopScores() const;

    ///
    /// \brief Loads the scores from the local text file.
    ///
    void loadScores();

    ///
    /// \brief Saves the current top scores to the local text file.
    ///
    void saveScores();
};

#endif // LEADERBOARDMODEL_H
