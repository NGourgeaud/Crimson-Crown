#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include "gamemodel.h"
#include "gameview.h"

class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(QWidget *parentWidget = nullptr, QObject *parent = nullptr);
    ~GameController();

    QWidget *createView(HeroType hero);
    void destroyView();

signals:
    void menuRequested();
    void gameOver(int score);
    void gameWon(int score);

private slots:
    void onPlayerDeath();
    void onBossDeath();
    void onTick();
    void onKeyPressed(int key);
    void onKeyReleased(int key);

private:
    GameModel *itsModel;
    GameView *itsView;
    QWidget *parentWidget;
    QTimer *itsTimer;
};

#endif // GAMECONTROLLER_H
