#include <QtTest>
#include <QFile>
#include "leaderboardmodel.h"

class TestLeaderboard : public QObject
{
    Q_OBJECT

private:
    LeaderboardModel* board;

    QString realFilePath = "scores.txt";
    QString backupFilePath = "scores_test.txt";

private slots:

    void initTestCase() {
        if (QFile::exists(realFilePath)) {
            QFile::rename(realFilePath, backupFilePath);
        }
    }

    void init() {
        board = new LeaderboardModel();
    }

    void cleanup() {
        delete board;
        QFile::remove(realFilePath);
    }

    void cleanupTestCase() {
        if (QFile::exists(realFilePath)) {
            QFile::remove(realFilePath);
        }
        if (QFile::exists(backupFilePath)) {
            QFile::rename(backupFilePath, realFilePath);
        }
        qDebug() << "Tests termines. Vraie sauvegarde restauree avec succes !";
    }

    void testEmptyBoard() {
        QVERIFY(board->getTopScores().isEmpty());
    }

    void testAddSingleScore() {
        board->addScore("Antoine", 500);
        QCOMPARE(board->getTopScores().size(), 1);
        QCOMPARE(board->getTopScores().first().playerName, QString("Antoine"));
    }

    // --- EDGE CASE ---

    void testSortingScores() {
        board->addScore("Antoine", 300);
        board->addScore("Arthur", 800);
        board->addScore("Quentin", 100);

        QCOMPARE(board->getTopScores().size(), 3);

        QCOMPARE(board->getTopScores().at(0).playerName, QString("Arthur"));
        QCOMPARE(board->getTopScores().at(0).score, 800);

        QCOMPARE(board->getTopScores().at(2).playerName, QString("Quentin"));
    }

    // Overflow test (Maximum array capacity)
    void testMaxCapacity() {
        // We simulate 15 played games
        for(int i = 0; i < 15; i++) {
            board->addScore("Joueur", 100 + i);
        }

        int maxScoresLimit = 10;
        QVERIFY(board->getTopScores().size() <= maxScoresLimit);
    }

    // Testing for extreme or empty data
    void testEmptyNameAndZeroScore() {
        board->addScore("", 0);

        QCOMPARE(board->getTopScores().size(), 1);
        QCOMPARE(board->getTopScores().first().playerName, QString(""));
        QCOMPARE(board->getTopScores().first().score, 0);
    }
};

QTEST_APPLESS_MAIN(TestLeaderboard)
#include "tst_testleaderboard.moc"
