#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include "gamemodel.h"

class GameView : public QWidget
{
    Q_OBJECT
public:
    explicit GameView(GameModel *model, QWidget *parent = nullptr);

signals:
    void keyPressed(int key);
    void keyReleased(int key);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    GameModel *itsModel;
};

#endif // GAMEVIEW_H
