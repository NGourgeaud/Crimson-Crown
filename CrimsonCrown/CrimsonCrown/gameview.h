 #ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QPointF>
#include <QPolygon>
#include <QList>
#include <QFrame>
#include "bossview.h"
#include "enemyview.h"
#include "gamemodel.h"
#include "hitzonemodel.h"
#include "hitzoneview.h"
#include "itemview.h"
#include "projectileview.h"
#include "roomview.h"
#include "obstacleview.h"
#include "playerview.h"
#include "collectibleview.h"

class GameView : public QWidget
{
    Q_OBJECT
public:
    explicit GameView(GameModel *model, QWidget *parent = nullptr);
    ~GameView();
    void setPauseMenuVisible(bool visible);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    GameModel *itsModel;
    RoomView *itsRoomView;
    ObstacleView *itsObstacleView;
    PlayerView *itsPlayerView;
    BossView * itsBossView;
    EnemyView * itsEnemyView;
    ProjectileView * itsProjectileView;
    HitZoneView * itsHitZoneView;
    ItemView * itsItemView;
    QList<HitZoneModel> itsAttackAreas;

    QPointF currentCameraCenter;
    bool isCameraInitialized;

    CollectibleView* itsChestView;

    QFrame *pauseMenu;

    QPixmap heartTexture;

signals:
    void keyPressed(int key);
    void keyReleased(int key);
    void quitRequested();

public slots:
    void drawAttackArea(HitZoneModel hitZone);
};

#endif // GAMEVIEW_H
