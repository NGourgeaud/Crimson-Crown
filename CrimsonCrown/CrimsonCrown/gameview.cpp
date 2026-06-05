#include "gameview.h"

GameView::GameView(GameModel *model, QWidget *parent)
    : QWidget(parent), itsModel(model), isCameraInitialized(false)
{
    setFocusPolicy(Qt::StrongFocus);

    itsRoomView = new RoomView();
    itsObstacleView = new ObstacleView();
    itsBossView = new BossView(LORD);

    if (itsModel->getItsPlayer() != nullptr) {
        itsPlayerView = new PlayerView(nullptr, itsModel->getItsPlayer());
        itsPlayerView->setItsTexture(HeroType::Blacksmith);
    } else {
        itsPlayerView = nullptr;
    }
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
        QRectF roomRect = room->getBounds(); // CORRECTION ICI

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

    // 1. DESSIN DU DONJON
    for (RoomModel* room : itsModel->getAllRooms()) {
        painter.save();
        painter.translate(room->getBounds().x(), room->getBounds().y()); // CORRECTION ICI
        itsRoomView->draw(painter, room, *itsObstacleView);
        painter.restore();
    }

    // 2. DESSIN DES ENNEMIS (Carrés rouges temporaires)
    painter.setBrush(QColor(220, 20, 60)); // Rouge "monstre"
    painter.setPen(QPen(Qt::black, 3));
    for (EnemyModel* enemy : itsModel->getItsEnemies()) {
        painter.drawRect(enemy->getHitBox());
    }
    const BossModel * boss = itsModel->getItsBoss();
    if (boss){
        itsBossView->draw(&painter, boss);
    }

    // 3. DESSIN DU JOUEUR
    if (itsPlayerView != nullptr) {
        itsPlayerView->draw(&painter);
    }

    // 4 DESSIN DES ZONES D'ATTAQUE
    if (!itsAttackAreas.empty()){
        for (QPolygon area : itsAttackAreas){
            painter.setBrush(QBrush(Qt::white));
            painter.setPen(QPen(Qt::red));
            painter.drawPolygon(area);
        }
        itsAttackAreas.clear();
    }

    painter.restore(); // Fin de la zone gérée par la caméra

    // ==========================================
    // INTERFACE DE JEU (HUD FIXE) - LE SCORE !
    // ==========================================
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    int boxWidth = 160;
    int boxHeight = 40;
    int spacing = 20;

    // Calcul pour centrer parfaitement les 2 boîtes en haut de l'écran
    int totalWidth = (2 * boxWidth) + spacing;
    int startX = (this->width() - totalWidth) / 2;
    int topY = 15;

    // --- 1. BOÎTE DE VIE (Rouge) ---
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

    // --- 2. BOÎTE DE SCORE (Gris foncé avec texte Doré/Blanc) ---
    QRect scoreRect(startX + boxWidth + spacing, topY, boxWidth, boxHeight);
    painter.setBrush(QColor(40, 40, 40));
    painter.setPen(Qt::NoPen);
    painter.drawRect(scoreRect);
    painter.setPen(QColor(255, 215, 0)); // Texte couleur Or pour faire ressortir le score
    painter.drawText(scoreRect, Qt::AlignCenter, QString("SCORE : %1").arg(itsModel->getItsScore()));

    // --- 3. ENNEMIS RESTANTS (En bas à gauche) ---
    painter.setPen(Qt::white);
    painter.setFont(QFont("Alagard", 18));
    int enemiesRemaining = itsModel->getItsEnemies().size();

    painter.drawText(20, this->height() - 45, QString("Ennemis restants : %1").arg(enemiesRemaining));
}
