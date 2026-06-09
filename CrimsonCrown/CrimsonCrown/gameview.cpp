#include "gameview.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

GameView::GameView(GameModel *model, QWidget *parent)
    : QWidget(parent), itsModel(model), isCameraInitialized(false)
{
    setFocusPolicy(Qt::StrongFocus);

    itsRoomView = new RoomView();
    itsObstacleView = new ObstacleView();
    itsBossView = new BossView;
    itsProjectileView = new ProjectileView;
    itsEnemyView = new EnemyView;

    if (itsModel->getItsPlayer() != nullptr) {
        itsPlayerView = new PlayerView(nullptr, itsModel->getItsPlayer());

        itsPlayerView->setItsTexture(itsModel->getItsPlayer()->getItsName());

    } else {
        itsPlayerView = nullptr;
    }

    // ==========================================
    // PAUSE MENU (Hidden by default)
    // ==========================================
    pauseMenu = new QFrame(this);
    // Center it on the 1200x800 window
    pauseMenu->setGeometry((WINDOW_WIDTH - 300) / 2, (WINDOW_HEIGHT - 350) / 2, 300, 350);
    pauseMenu->setStyleSheet("QFrame { background-color: rgba(20, 20, 20, 220); border: 2px solid #555555; border-radius: 10px; }");

    QVBoxLayout *pauseLayout = new QVBoxLayout(pauseMenu);
    pauseLayout->setContentsMargins(30, 30, 30, 30);
    pauseLayout->setSpacing(20);

    QLabel *pauseTitle = new QLabel("PAUSE");
    pauseTitle->setFont(QFont("Alagard", 35, QFont::Bold));
    pauseTitle->setStyleSheet("color: #FFD700; background: transparent; border: none;");
    pauseTitle->setAlignment(Qt::AlignCenter);
    pauseLayout->addWidget(pauseTitle);

    QString btnStyle =
        "QPushButton {"
        "   background-color: #111111;"
        "   color: white;"
        "   border: 2px solid #555555;"
        "   font-family: 'Alagard';"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border-radius: 3px;"
        "   height: 50px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #E30000;"
        "   border-color: white;"
        "}";

    QPushButton *resumeBtn = new QPushButton("RESUME");
    resumeBtn->setStyleSheet(btnStyle);
    // Trick: Clicking "Resume" simulates pressing Escape to remove the pause!
    connect(resumeBtn, &QPushButton::clicked, this, [this]() { emit keyPressed(Qt::Key_Escape); });
    pauseLayout->addWidget(resumeBtn);

    QPushButton *quitBtn = new QPushButton("QUIT GAME");
    quitBtn->setStyleSheet(btnStyle);
    connect(quitBtn, &QPushButton::clicked, this, &GameView::quitRequested);
    pauseLayout->addWidget(quitBtn);

    pauseMenu->hide(); // Hide it at startup
}

GameView::~GameView()
{
    delete itsRoomView;
    delete itsObstacleView;
    if (itsPlayerView != nullptr) {
        delete itsPlayerView;
    }
}

void GameView::keyPressEvent(QKeyEvent *event) { if (!event->isAutoRepeat()) emit keyPressed(event->key()); }
void GameView::keyReleaseEvent(QKeyEvent *event) { if (!event->isAutoRepeat()) emit keyReleased(event->key()); }

void GameView::drawAttackArea(QPolygon hitbox)
{
    itsAttackAreas.append(hitbox);
    update();

    QTimer::singleShot(100, this, [this, hitbox]() {
        itsAttackAreas.removeOne(hitbox);
    });
}

void GameView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.fillRect(this->rect(), Qt::black);

    if (itsModel->getItsPlayer() == nullptr) return;

    QPointF playerCenter = itsModel->getItsPlayer()->getHitBox().center();
    QPointF cameraTarget = playerCenter;

    for (RoomModel* room : itsModel->getAllRooms()) {
        QRectF roomRect = room->getBounds(); // CORRECTION HERE

        if (roomRect.contains(playerCenter)) {
            qreal minX = roomRect.left() + (this->width() / 2.0);
            qreal maxX = roomRect.right() - (this->width() / 2.0);
            qreal minY = roomRect.top() + (this->height() / 2.0);
            qreal maxY = roomRect.bottom() - (this->height() / 2.0);

            if (minX <= maxX) cameraTarget.setX(qBound(minX, playerCenter.x(), maxX));
            else cameraTarget.setX(roomRect.center().x());

            if (minY <= maxY) cameraTarget.setY(qBound(minY, playerCenter.y(), maxY));
            else cameraTarget.setY(roomRect.center().y());
            break;
        }
    }

    if (!isCameraInitialized) {
        currentCameraCenter = cameraTarget;
        isCameraInitialized = true;
    }

    qreal smoothing = 0.15;
    currentCameraCenter.setX(currentCameraCenter.x() + (cameraTarget.x() - currentCameraCenter.x()) * smoothing);
    currentCameraCenter.setY(currentCameraCenter.y() + (cameraTarget.y() - currentCameraCenter.y()) * smoothing);

    QPointF cameraOffset(this->width() / 2.0 - currentCameraCenter.x(),
                         this->height() / 2.0 - currentCameraCenter.y());

    painter.save();
    painter.translate(cameraOffset);

    // 1. DRAW DUNGEON
    for (RoomModel* room : itsModel->getAllRooms()) {
        painter.save();
        painter.translate(room->getBounds().x(), room->getBounds().y()); // CORRECTION HERE
        itsRoomView->draw(painter, room, *itsObstacleView, itsModel->getCurrentLevel());
        painter.restore();
    }

    if (!itsModel->areAllCombatRoomsCleared()) {
        for (RoomModel* room : itsModel->getAllRooms()) {
            if (room->getType() == RoomType::Boss) {
                painter.save();
                painter.translate(room->getBounds().x(), room->getBounds().y());

                // Draw a large brown beam (solid wood color)
                painter.setBrush(QColor(139, 69, 19));
                painter.setPen(QPen(Qt::black, 3));

                // Place it right at the south entrance of the Boss room
                int doorX = (room->getWidth() / 2) - 75; // The corridor is 150 wide
                int doorY = room->getHeight() - 20;      // At the very bottom of the room
                painter.drawRect(doorX, doorY, 150, 20);

                painter.restore();
            }
        }
    }

    // 2. DRAW ENEMIES
    if (!itsModel->getItsEnemies().empty()){
        for (EnemyModel* enemy : itsModel->getItsEnemies()) {
            itsEnemyView->setItsTexture(enemy->getitsEnemyType());
            itsEnemyView->draw(&painter, enemy);
        }
    }

    if(itsModel->getItsBoss()){
        const BossModel * theBoss = itsModel->getItsBoss();
        if (theBoss){
            itsBossView->setItsTexture(theBoss->getItsType());
            itsBossView->draw(&painter, theBoss);
        }
    }

    // 3. DRAW PLAYER
    if (itsPlayerView != nullptr) {
        itsPlayerView->draw(&painter);
    }

    // 4 DRAW ATTACK AREAS
    if (!itsAttackAreas.empty()){
        for (QPolygon area : itsAttackAreas){
            painter.setBrush(QBrush(Qt::white));
            painter.setPen(QPen(Qt::red));
            painter.drawPolygon(area);
        }
    }

    // draw projectile
    if (!itsModel->getItsProjeciles().empty()){
        for (ProjectileModel * projectile : itsModel->getItsProjeciles()){

            itsProjectileView->setItsTexture(projectile->getItsType());
            itsProjectileView->draw(&painter, projectile);
        }
    }

    painter.restore(); // End of camera-managed area

    // ==========================================
    // GAME INTERFACE (STATIC HUD) - THE SCORE!
    // ==========================================
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    int boxWidth = 160;
    int boxHeight = 40;
    int spacing = 20;

    // Calculation to perfectly center the 2 boxes at the top of the screen
    int totalWidth = (2 * boxWidth) + spacing;
    int startX = (this->width() - totalWidth) / 2;
    int topY = 15;

    // --- 1. HEALTH BOX (Red) ---
    PlayerModel* player = itsModel->getItsPlayer();
    int currentHP = 0, maxHP = 40;
    if (player != nullptr) {
        currentHP = player->getHP();
        maxHP = player->getMaxHP();
        if (currentHP < 0) currentHP = 0;
    }

    QRect hpRect(startX, topY, boxWidth, boxHeight);
    painter.setBrush(QColor(230, 40, 40));
    painter.setPen(Qt::NoPen);
    painter.drawRect(hpRect);
    painter.setPen(Qt::white);
    painter.drawText(hpRect, Qt::AlignCenter, QString("♥ %1 / %2").arg(currentHP).arg(maxHP));

    // --- 2. SCORE BOX (Dark gray with Gold/White text) ---
    QRect scoreRect(startX + boxWidth + spacing, topY, boxWidth, boxHeight);
    painter.setBrush(QColor(40, 40, 40));
    painter.setPen(Qt::NoPen);
    painter.drawRect(scoreRect);
    painter.setPen(QColor(255, 215, 0)); // Gold color text to make the score stand out
    painter.drawText(scoreRect, Qt::AlignCenter, QString("SCORE : %1").arg(itsModel->getItsScore()));

    // --- 3. REMAINING ENEMIES (Bottom left) ---
    painter.setPen(Qt::white);
    painter.setFont(QFont("Alagard", 18));
    int enemiesRemaining = itsModel->getItsEnemies().size();

    painter.drawText(20, this->height() - 45, QString("Ennemis restants : %1").arg(enemiesRemaining));
}

void GameView::setPauseMenuVisible(bool visible)
{
    if (visible) {
        pauseMenu->show();
    } else {
        pauseMenu->hide();
        // VERY IMPORTANT: Give focus back to the game, otherwise the keyboard stops responding!
        this->setFocus();
    }
}
