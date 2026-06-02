#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "gamemodel.h"
#include "gameview.h"
#include <QObject>

class GameController : public QObject
{
    Q_OBJECT
private:
    GameModel * itsModel;
    GameView *  itsView;

public:
    GameController();
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private slots:
    void onKeyPressed(int key);
    void onKeyReleased(int key);
};

#endif // GAMECONTROLLER_H
