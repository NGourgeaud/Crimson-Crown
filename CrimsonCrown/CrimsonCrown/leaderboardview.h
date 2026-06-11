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
    void paintEvent(QPaintEvent *event) override;

public:
    explicit LeaderboardView(QWidget *parent, const QList<ScoreEntry>& scores);

signals:
    void backRequested();
};

#endif // LEADERBOARDVIEW_H
