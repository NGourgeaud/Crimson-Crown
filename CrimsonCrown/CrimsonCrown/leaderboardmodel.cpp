// File Name: leaderboardmodel.cpp
// Group: A11
// Description: Data model handling the persistent storage, loading, and sorting of top player scores using a text file.

#include "leaderboardmodel.h"
#include <QFile>
#include <QTextStream>
#include <algorithm>

///
/// \brief Constructs the leaderboard model and loads existing scores from disk.
///
LeaderboardModel::LeaderboardModel() {
    loadScores();
}

///
/// \brief Adds a new score to the leaderboard, sorts the list, and saves it.
/// \param name The name of the player.
/// \param score The integer score achieved.
/// \details Maintains a maximum of 10 entries by removing the lowest score if exceeded.
///
void LeaderboardModel::addScore(const QString& name, int score) {
    topScores.append({name, score});

    // Sorts the scores in descending order
    std::sort(topScores.begin(), topScores.end(), [](const ScoreEntry& playerA, const ScoreEntry& playerB) {
        return playerA.score > playerB.score;
    });

    // We keep only the Top 10
    if (topScores.size() > 10) {
        topScores.removeLast();
    }
    saveScores();
}

///
/// \brief Retrieves the list of the top sorted scores.
/// \return A QList containing ScoreEntry structures.
///
QList<ScoreEntry> LeaderboardModel::getTopScores() const {
    return topScores;
}

///
/// \brief Loads the scores from the local text file into memory.
///
void LeaderboardModel::loadScores() {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 2) {
                topScores.append({parts[0], parts[1].toInt()});
            }
        }
        file.close();
    }
}

///
/// \brief Saves the current list of top scores to the local text file.
///
void LeaderboardModel::saveScores() {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        for (const ScoreEntry& entry : topScores) {
            out << entry.playerName << "," << entry.score << "\n";
        }

        file.close();
    }
}
