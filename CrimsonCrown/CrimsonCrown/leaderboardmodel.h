#ifndef LEADERBOARDMODEL_H
#define LEADERBOARDMODEL_H

#include <QString>
#include <QList>

struct ScoreEntry {
    QString playerName;
    int score;
};

class LeaderboardModel {
private:
    QList<ScoreEntry> topScores;
    const QString filePath = "scores.txt";

public:
    LeaderboardModel();

    void addScore(const QString& name, int score);
    QList<ScoreEntry> getTopScores() const;

private:
    void loadScores();
    void saveScores();
};

#endif // LEADERBOARDMODEL_H
