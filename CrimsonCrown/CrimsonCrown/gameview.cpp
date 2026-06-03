#include "gameview.h"

GameView::GameView(GameModel *model, QWidget *parent)
    : QWidget(parent), itsModel(model), isCameraInitialized(false)
{
    setFocusPolicy(Qt::StrongFocus);

    itsRoomView = new RoomView();
    itsObstacleView = new ObstacleView();

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

void GameView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // Lissage des textures (Anti-Aliasing)
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Fond global (Ténèbres)
    QPixmap background(":/fondjeu.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), background);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 150));

    if (itsModel->getItsPlayer() == nullptr) return;

    // ==========================================
    // CAMÉRA DYNAMIQUE LERP
    // ==========================================
    QPointF playerCenter = itsModel->getItsPlayer()->getHitBox().center();
    QPointF cameraTarget = playerCenter;

    // Cherche dans quelle salle on est pour faire le confinement (Clamping)
    for (RoomModel* room : itsModel->getAllRooms()) {
        QRectF roomRect(room->getOffsetX(), room->getOffsetY(), room->getWidth(), room->getHeight());

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

    // Le fameux "élastique"
    qreal smoothing = 0.15;
    currentCameraCenter.setX(currentCameraCenter.x() + (cameraTarget.x() - currentCameraCenter.x()) * smoothing);
    currentCameraCenter.setY(currentCameraCenter.y() + (cameraTarget.y() - currentCameraCenter.y()) * smoothing);

    QPointF cameraOffset(this->width() / 2.0 - currentCameraCenter.x(),
                         this->height() / 2.0 - currentCameraCenter.y());

    painter.save();
    painter.translate(cameraOffset);

    // DESSIN DU DONJON (Avec ses bordures bien nettes)
    for (RoomModel* room : itsModel->getAllRooms()) {
        painter.save();
        painter.translate(room->getOffsetX(), room->getOffsetY());
        itsRoomView->draw(painter, room, *itsObstacleView);
        painter.restore();
    }

    // DESSIN DU JOUEUR
    if (itsPlayerView != nullptr) {
        itsPlayerView->draw(&painter);
    }

    painter.restore();
}