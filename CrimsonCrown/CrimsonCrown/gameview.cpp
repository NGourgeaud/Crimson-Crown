#include "gameview.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

GameView::GameView(GameModel *model, QWidget *parent)
    : QWidget(parent), itsModel(model), isCameraInitialized(false)
{

    itsRoomView = new RoomView();
    itsObstacleView = new ObstacleView();
    itsBossView = new BossView;
    itsProjectileView = new ProjectileView;
    itsEnemyView = new EnemyView;
    itsChestView = new CollectibleView();
    itsHitZoneView = new HitZoneView;
    itsItemView = new ItemView;

    heartTexture = QPixmap(":/coeur.png");
    

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

    pauseMenu->setFixedSize(300, 350);
    pauseMenu->setStyleSheet("QFrame { background-color: rgba(20, 20, 20, 220); border: 2px solid #555555; border-radius: 10px; }");

    // --- LE LAYOUT INTERNE DU MENU (Inchangé) ---
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

    QPushButton *resumeBtn = new QPushButton("REPRENDRE");
    resumeBtn->setStyleSheet(btnStyle);
    connect(resumeBtn, &QPushButton::clicked, this, [this]() { emit keyPressed(Qt::Key_Escape); });
    pauseLayout->addWidget(resumeBtn);

    QPushButton *quitBtn = new QPushButton("QUIITER");
    quitBtn->setStyleSheet(btnStyle);
    connect(quitBtn, &QPushButton::clicked, this, &GameView::quitRequested);
    pauseLayout->addWidget(quitBtn);

    pauseMenu->hide();

    QVBoxLayout *overlayLayout = new QVBoxLayout(this);
    overlayLayout->addWidget(pauseMenu, 0, Qt::AlignCenter);

    setupShopMenu();
    overlayLayout->addWidget(shopMenu, 0, Qt::AlignCenter);
}

void GameView::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    this->setFocus();
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

void GameView::drawAttackArea(HitZoneModel hitZone)
{
    itsAttackAreas.append(hitZone);
    update();

    QTimer::singleShot(100, this, [this, hitZone]() {
        itsAttackAreas.removeOne(hitZone);
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
        itsRoomView->draw(painter, room, *itsObstacleView);
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

    // --- DESSIN DU COFFRE EASTER EGG ---
    // Le coffre doit être au centre de la salle de loot (qui commence à x=1250, y=2075 et fait 600x600).
    // Centre = (1250 + 600/2, 2075 + 600/2) = (1550, 2375).
    // Si ton coffre fait 100x100 pixels, il faut le décaler de la moitié (50, 50).
    // Coordonnées de départ : (1500, 2325).
    itsChestView->setBounds(1500, 2325, 100, 100);

    // On dessine le coffre en mode "fermé" (vrai) tant que le joueur n'a pas la clef.
    // (Puisque dans ton code GameModel::updateLogic(), on supprime la clef quand le coffre s'ouvre)
    bool isClosed = itsModel->getItsPlayer()->getHasSecretKey();
    itsChestView->draw(painter, isClosed);

    // --- DESSIN DES COEURS DE SOIN (NIVEAU 2) ---
    if (!itsModel->getItsItems().empty()){
        for (ItemModel * item : itsModel->getItsItems()){
            itsItemView->draw(&painter, item);
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
        for (HitZoneModel hitzone : itsAttackAreas){
            itsHitZoneView->draw(&painter, &hitzone);
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

    // Calcul pour centrer parfaitement 3 boîtes en haut de l'écran (Vie, Score, Or)
    int totalWidth = (3 * boxWidth) + (2 * spacing);
    int startX = (this->width() - totalWidth) / 2;
    int topY = 15;

    // --- LEVEL ---
    if (itsModel != nullptr) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 16, QFont::Bold));
        QString levelText = "NIVEAU " + QString::number(itsModel->getCurrentLevel());
        painter.drawText(20, 40, levelText);
    }

    // --- 1. HEALTH BOX (Rouge) ---
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
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(hpRect, Qt::AlignCenter, QString("♥ %1 / %2").arg(currentHP).arg(maxHP));

    // --- 2. SCORE BOX  ---
    QRect scoreRect(startX + boxWidth + spacing, topY, boxWidth, boxHeight);
    painter.setBrush(QColor(40, 40, 40));
    painter.setPen(Qt::NoPen);
    painter.drawRect(scoreRect);
    painter.setPen(QColor(255, 215, 0));
    painter.drawText(scoreRect, Qt::AlignCenter, QString("SCORE : %1").arg(itsModel->getItsScore()));

    // --- 3. GOLD BOX  ---
    QRect goldRect(startX + 2 * (boxWidth + spacing), topY, boxWidth, boxHeight);
    painter.setBrush(QColor(239, 191, 4));
    painter.setPen(Qt::NoPen);
    painter.drawRect(goldRect);
    painter.setPen(Qt::white);
    painter.drawText(goldRect, Qt::AlignCenter, QString("OR : %1").arg(itsModel->getGoldAmount()));


    // --- 4. REMAINING ENEMIES ---
    painter.setPen(Qt::white);
    painter.setFont(QFont("Alagard", 18));
    int enemiesRemaining = itsModel->getItsEnemies().size();

    painter.drawText(20, this->height() - 45, QString("Ennemis restants : %1").arg(enemiesRemaining));

    // ==========================================
    //  5. COOLDOWN BAR
    // ==========================================
    if (player != nullptr) {
        int c1Max = 1, c2Max = 1;
        QString name1 = "Attaque 1", name2 = "Attaque 2";

        switch(player->getItsName()) {
        case HeroType::Assassin:
            c1Max = static_cast<int>(AssassinAttack1Cooldown * 1000.0f);
            c2Max = static_cast<int>(AssassinAttack2Cooldown * 1000.0f);
            name1 = "Couteau";
            name2 = "Fumée";
            break;
        case HeroType::Hunter:
            c1Max = static_cast<int>(HunterAttack1Cooldown * 1000.0f);
            c2Max = static_cast<int>(HunterAttack2Cooldown * 1000.0f);
            name1 = "Flèche";
            name2 = "Zone Piège";
            break;
        case HeroType::Blacksmith:
            c1Max = static_cast<int>(BlacksmithAttack1Cooldown * 1000.0f);
            c2Max = static_cast<int>(BlacksmithAttack2Cooldown * 1000.0f);
            name1 = "Marteau";
            name2 = "Fracas";
            break;
        default:
            break;
        }

        int barWidth = 150;
        int barHeight = 22;
        int spacing = 15;
        int marginRight = 30;

        int totalWidth = (2 * barWidth) + spacing;
        int hudX = this->width() - totalWidth - marginRight;

        int hudY = this->height() - 45;

        // ------------------------------------------
        // --- MAIN ATTACK
        // ------------------------------------------
        QRect bar1Rect(hudX, hudY, barWidth, barHeight);
        painter.setBrush(QColor(40, 40, 40, 200));
        painter.setPen(QPen(Qt::white, 1));
        painter.drawRect(bar1Rect);

        double progress1 = 1.0;
        if (!player->getCanAttack1() && c1Max > 0) {
            double tempsRestant1 = static_cast<double>(player->getCooldown1Remaining());
            progress1 = 1.0 - (tempsRestant1 / c1Max);
            if (progress1 < 0.0) progress1 = 0.0;
            if (progress1 > 1.0) progress1 = 1.0;
        }

        if (progress1 > 0) {
            painter.setBrush(QColor(46, 204, 113)); // Green
            painter.setPen(Qt::NoPen);
            painter.drawRect(hudX + 1, hudY + 1, static_cast<int>((barWidth - 2) * progress1), barHeight - 2);
        }

        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 10, QFont::Bold));
        painter.drawText(bar1Rect, Qt::AlignCenter, name1);


        // ------------------------------------------
        // --- SPECIAL ATTACK
        // ------------------------------------------
        int bar2X = hudX + barWidth + spacing;

        QRect bar2Rect(bar2X, hudY, barWidth, barHeight);
        painter.setBrush(QColor(40, 40, 40, 200));
        painter.setPen(QPen(Qt::white, 1));
        painter.drawRect(bar2Rect);

        double progress2 = 1.0;
        if (!player->getCanAttack2() && c2Max > 0) {
            double tempsRestant2 = static_cast<double>(player->getCooldown2Remaining());
            progress2 = 1.0 - (tempsRestant2 / c2Max);
            if (progress2 < 0.0 or progress2 == 1.0) progress2 = 0.0;
            if (progress2 > 1.0) progress2 = 1.0;
        }

        if (progress2 > 0) {
            painter.setBrush(QColor(52, 152, 219)); // Blue
            painter.setPen(Qt::NoPen);
            painter.drawRect(bar2X + 1, hudY + 1, static_cast<int>((barWidth - 2) * progress2), barHeight - 2);
        }

        painter.setPen(Qt::white);
        painter.drawText(bar2Rect, Qt::AlignCenter, name2);
    }
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

void GameView::setupShopMenu() {
    shopMenu = new QFrame(this);
    shopMenu->setFixedSize(400, 450);
    shopMenu->setStyleSheet("QFrame { background-color: rgba(20, 20, 20, 240); border: 2px solid #FFD700; border-radius: 10px; }");

    QVBoxLayout *layout = new QVBoxLayout(shopMenu);

    QLabel *title = new QLabel("BOUTIQUE");
    title->setFont(QFont("Alagard", 30, QFont::Bold));
    title->setStyleSheet("color: #FFD700; border: none; background: transparent;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    shopGoldLabel = new QLabel("Or disponible : 0");
    shopGoldLabel->setStyleSheet("color: white; border: none; background: transparent; font-size: 18px;");
    layout->addWidget(shopGoldLabel);

    // Style des boutons
    QString btnStyle = "QPushButton { background-color: #111; color: white; border: 1px solid #555; padding: 10px; }"
                       "QPushButton:hover { background-color: #333; border-color: #FFD700; }";

    btnHP = new QPushButton(); // On laisse le texte vide pour l'instant
    btnHP->setStyleSheet(btnStyle);
    connect(btnHP, &QPushButton::clicked, this, [this]() { emit buyUpgradeRequested("HP"); });
    layout->addWidget(btnHP);

    btnDmg = new QPushButton();
    btnDmg->setStyleSheet(btnStyle);
    connect(btnDmg, &QPushButton::clicked, this, [this]() { emit buyUpgradeRequested("DMG"); });
    layout->addWidget(btnDmg);

    btnSpeed = new QPushButton();
    btnSpeed->setStyleSheet(btnStyle);
    connect(btnSpeed, &QPushButton::clicked, this, [this]() { emit buyUpgradeRequested("SPEED"); });
    layout->addWidget(btnSpeed);

    QPushButton *btnNext = new QPushButton("NIVEAU SUIVANT");
    btnNext->setStyleSheet("background-color: #E30000; color: white; padding: 15px; font-weight: bold; border-radius: 5px; font-family: 'Alagard'; font-size: 20px;");
    connect(btnNext, &QPushButton::clicked, this, &GameView::nextLevelRequested);
    layout->addWidget(btnNext);

    shopMenu->hide();
}

void GameView::setShopMenuVisible(bool visible)
{
    if (shopMenu == nullptr || shopGoldLabel == nullptr) return;

    if (visible) {
        int level = itsModel->getCurrentLevel();
        double multiplier = std::pow(PRICE_SCALE, level - 2);

        int currentPriceHP = static_cast<int>(BASE_PRICE_HP * multiplier);
        int currentPriceDmg = static_cast<int>(BASE_PRICE_DMG * multiplier);
        int currentPriceSpeed = static_cast<int>(BASE_PRICE_SPEED * multiplier);

        btnHP->setText(QString("+5 Max HP (Prix: %1 Or)").arg(currentPriceHP));
        btnDmg->setText(QString("+20% Dégâts (Prix: %1 Or)").arg(currentPriceDmg));
        btnSpeed->setText(QString("+2 Vitesse (Prix: %1 Or)").arg(currentPriceSpeed));

        updateShopGold(itsModel->getGoldAmount());
        shopMenu->show();
    } else {
        shopMenu->hide();
        this->setFocus();
    }
}

void GameView::updateShopGold(int amount) {
    shopGoldLabel->setText(QString("Or disponible : %1").arg(amount));
}
