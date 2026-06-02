#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "gamemodel.h"
#include <QObject>
#include <QKeyEvent>

class GameView : public QObject
{
    Q_OBJECT
private:
    GameModel * itsModel;

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

public:
    GameView(GameModel *aModel);

signals:
    void keyPressed(int key);
    void keyReleased(int key);
};

#endif // GAMEVIEW_H
