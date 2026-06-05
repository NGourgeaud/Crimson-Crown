#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QPointF>
#include <QPolygon>
#include <QList>
#include "gamemodel.h"
#include "roomview.h"
#include "obstacleview.h"
#include "playerview.h"

class GameView : public QWidget
{
    Q_OBJECT
public:
    explicit GameView(GameModel *model, QWidget *parent = nullptr);
    ~GameView();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    GameModel *itsModel;
    RoomView *itsRoomView;
    ObstacleView *itsObstacleView;
    PlayerView *itsPlayerView;
    QList<QPolygon> itsAttackAreas;

    QPointF currentCameraCenter;
    bool isCameraInitialized;

signals:
    void keyPressed(int key);
    void keyReleased(int key);
public slots:
    void drawAttackArea(QPolygon hitbox);
};

#endif // GAMEVIEW_H
