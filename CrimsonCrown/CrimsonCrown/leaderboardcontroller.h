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
    explicit LeaderboardController(QWidget *parentWidget, QObject *parent = nullptr);
    ~LeaderboardController();

    QWidget* createView();
    void destroyView();

    void submitScore(const QString& playerName, int score);

signals:
    void menuRequested();
};

#endif // LEADERBOARDCONTROLLER_H
