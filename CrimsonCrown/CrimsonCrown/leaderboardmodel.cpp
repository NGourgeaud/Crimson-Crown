#include "leaderboardmodel.h"
#include <QFile>
#include <QTextStream>
#include <algorithm>

LeaderboardModel::LeaderboardModel() {
    loadScores();
}

void LeaderboardModel::addScore(const QString& name, int score) {
    topScores.append({name, score});

    std::sort(topScores.begin(), topScores.end(), [](const ScoreEntry& joueurA, const ScoreEntry& joueurB) {
        return joueurA.score > joueurB.score;
    });

    // We're keeping the Top 10
    if (topScores.size() > 10) {
        topScores.removeLast();
    }
    saveScores();
}

QList<ScoreEntry> LeaderboardModel::getTopScores() const {
    return topScores;
}

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
