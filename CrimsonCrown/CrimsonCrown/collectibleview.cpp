#include "collectibleview.h"

CollectibleView::CollectibleView()
{
    // CHARGE TON IMAGE ICI !
    // Remplace "coffre.png" par le vrai nom de ton PNG dans ton .qrc
    chestTexture = QPixmap(":/coffre-easterEgg.png");
}

void CollectibleView::setBounds(qreal x, qreal y, qreal width, qreal height)
{
    bounds = QRectF(x, y, width, height);
}

void CollectibleView::draw(QPainter &painter, bool isChestClosed)
{
    if (chestTexture.isNull()) return;

    // Si on a besoin de montrer que le coffre est déjà ouvert,
    // on peut le dessiner un peu transparent par exemple.
    if (!isChestClosed) {
        painter.save();
        painter.setOpacity(0.4); // 40% opaque
    }

    // On dessine l'image en lui demandant de remplir toute la hitbox définie.
    painter.drawPixmap(bounds.toRect(), chestTexture);

    if (!isChestClosed) {
        painter.restore();
    }
}