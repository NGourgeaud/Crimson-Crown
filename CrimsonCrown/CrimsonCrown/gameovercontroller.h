#ifndef GAMEOVERCONTROLLER_H
#define GAMEOVERCONTROLLER_H

#include <QObject>
#include "gameoverview.h"

class GameOverController : public QObject {
    Q_OBJECT
public:
    explicit GameOverController(QWidget *parentWidget = nullptr, QObject *parent = nullptr);

    QWidget* createView(int score, HeroType hero);
    void destroyView();

signals:
    void menuRequested();

private:
    GameOverView *itsView;
    QWidget *parentWidget;
};

#endif // GAMEOVERCONTROLLER_H
