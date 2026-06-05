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

    if (!itsModel->areAllCombatRoomsCleared()) {
        for (RoomModel* room : itsModel->getAllRooms()) {
            if (room->getType() == RoomType::Boss) {
                painter.save();
                painter.translate(room->getBounds().x(), room->getBounds().y());

                // On dessine une grosse poutre marron (couleur bois massif)
                painter.setBrush(QColor(139, 69, 19));
                painter.setPen(QPen(Qt::black, 3));

                // On la place pile à l'entrée sud de la salle du Boss
                int doorX = (room->getWidth() / 2) - 75; // Le couloir fait 150 de large
                int doorY = room->getHeight() - 20;      // Tout en bas de la salle
                painter.drawRect(doorX, doorY, 150, 20);

                painter.restore();
            }
        }
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
    painter.setPen(Qt::white);
    painter.setFont(QFont("Alagard", 22, QFont::Bold));

    QString scoreText = QString("SCORE : %1").arg(itsModel->getItsScore());
    painter.drawText(20, 40, scoreText);
}
