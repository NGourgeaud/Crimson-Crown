// File Name: gameview.h
// Group: A11
// Description: Header file defining the main rendering engine and HUD for the active game session.

#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QPointF>
#include <QPolygon>
#include <QList>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
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
    ///
    /// \brief Initializes the game view and allocates required sub-views.
    /// \param model Pointer to the game model.
    /// \param parent The parent QWidget.
    ///
    explicit GameView(GameModel *model, QWidget *parent = nullptr);

    ///
    /// \brief Destructor for freeing allocated views.
    ///
    ~GameView();

    ///
    /// \brief Toggles the visibility of the pause menu.
    /// \param visible True to show the menu, false to hide.
    ///
    void setPauseMenuVisible(bool visible);

    ///
    /// \brief Toggles the visibility of the in-game shop menu.
    /// \param visible True to show the shop, false to hide.
    ///
    void setShopMenuVisible(bool visible);

    ///
    /// \brief Updates the gold counter displayed in the shop.
    /// \param amount The current amount of gold.
    ///
    void updateShopGold(int amount);

signals:
    void keyPressed(int key);
    void keyReleased(int key);
    void quitRequested();
    void buyUpgradeRequested(QString type);
    void nextLevelRequested();

public slots:
    ///
    /// \brief Receives a hit zone to render it briefly on screen.
    /// \param hitZone The hit zone data to draw.
    ///
    void drawAttackArea(HitZoneModel hitZone);

protected:
    ///
    /// \brief Renders the entire game view (rooms, entities, HUD).
    /// \param event The paint event.
    ///
    void paintEvent(QPaintEvent *event) override;

    ///
    /// \brief Captures key presses.
    /// \param event The key event.
    ///
    void keyPressEvent(QKeyEvent *event) override;

    ///
    /// \brief Captures key releases.
    /// \param event The key event.
    ///
    void keyReleaseEvent(QKeyEvent *event) override;

    ///
    /// \brief Ensures the widget takes focus when shown.
    /// \param event The show event.
    ///
    void showEvent(QShowEvent *event) override;

    ///
    /// \brief Configures the layout and components of the shop menu.
    ///
    void setupShopMenu();

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

    QFrame *shopMenu;
    QLabel *shopGoldLabel;

    QPushButton *btnHP;
    QPushButton *btnDmg;
    QPushButton *btnSpeed;
};

#endif // GAMEVIEW_H
