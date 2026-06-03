#include "gameview.h"

///
/// \brief GameView::GameView
///
GameView::GameView(GameModel *model, QWidget *parent)
    : QWidget(parent), itsModel(model)
{
    setFocusPolicy(Qt::StrongFocus);
    isCameraInitialized = false;

    // 1. Instanciation des vues statiques
    itsRoomView = new RoomView();
    itsObstacleView = new ObstacleView();

    // 2. Instanciation de la vue du joueur
    if (itsModel->getItsPlayer() != nullptr) {
        itsPlayerView = new PlayerView(nullptr, itsModel->getItsPlayer());
        itsPlayerView->setItsTexture(HeroType::Blacksmith);
    } else {
        itsPlayerView = nullptr;
    }
}

///
/// \brief GameView::~GameView
///
GameView::~GameView()
{
    delete itsRoomView;
    delete itsObstacleView;
    if (itsPlayerView != nullptr) {
        delete itsPlayerView;
    }
}

void GameView::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        emit keyPressed(event->key());
    }
}

void GameView::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        emit keyReleased(event->key());
    }
}

///
/// \brief GameView::paintEvent
///
void GameView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // --- OPTIMISATION FLUIDITÉ ---
    // Force Qt à lisser les pixels lors des déplacements
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // 1. FOND D'ÉCRAN GLOBAL
    QPixmap background(":/fondjeu.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), background);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 150));

    if (itsModel->getItsPlayer() == nullptr) return;

    // ==========================================
    // 2. LA CAMÉRA FLUIDE (LERP) FAÇON SOUL KNIGHT
    // ==========================================
    QPointF playerCenter = itsModel->getItsPlayer()->getHitBox().center();
    QPointF cameraTarget = playerCenter; // C'est là que la caméra VEUT aller

    // On calcule la cible (Le confinement contre les murs)
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

    // -- LA MAGIE DU LISSAGE (L'ÉLASTIQUE) --

    // Si le jeu vient de se lancer, on téléporte la caméra direct sur le joueur
    if (!isCameraInitialized) {
        currentCameraCenter = cameraTarget;
        isCameraInitialized = true;
    }

    // Le Lerp : La caméra fait 15% du chemin vers sa cible à chaque frame (60 fois par seconde)
    qreal smoothing = 0.25; // Modifie cette valeur ! 0.05 = Caméra très lente, 0.5 = Très rapide
    currentCameraCenter.setX(currentCameraCenter.x() + (cameraTarget.x() - currentCameraCenter.x()) * smoothing);
    currentCameraCenter.setY(currentCameraCenter.y() + (cameraTarget.y() - currentCameraCenter.y()) * smoothing);

    // Calcul du décalage final pour dessiner
    QPointF cameraOffset(this->width() / 2.0 - currentCameraCenter.x(),
                         this->height() / 2.0 - currentCameraCenter.y());

    painter.save();
    painter.translate(cameraOffset);

    // 3. DESSIN DE LA CARTE COMPLÈTE
    for (RoomModel* room : itsModel->getAllRooms()) {
        painter.save();
        painter.translate(room->getOffsetX(), room->getOffsetY());
        itsRoomView->draw(painter, room, *itsObstacleView);
        painter.restore();
    }

    // 4. DESSIN DU JOUEUR
    if (itsPlayerView != nullptr) {
        itsPlayerView->draw(&painter);
    }

    painter.restore();
}