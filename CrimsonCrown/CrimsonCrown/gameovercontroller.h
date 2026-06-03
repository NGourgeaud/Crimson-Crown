#ifndef GAMEOVERCONTROLLER_H
#define GAMEOVERCONTROLLER_H

#include <QObject>
#include "gameoverview.h"

class GameOverController : public QObject {
    Q_OBJECT
public:
    explicit GameOverController(QWidget *parentWidget = nullptr, QObject *parent = nullptr);

    QWidget *createView(int score);
    void destroyView();

signals:
    void backToMenuRequested();

private:
    GameOverView *itsView;
    QWidget *parentWidget;
};

#endif // GAMEOVERCONTROLLER_H
