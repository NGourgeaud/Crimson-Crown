#ifndef COLLECTIBLEVIEW_H
#define COLLECTIBLEVIEW_H

#include <QPainter>
#include <QPixmap>
#include <QRectF>

class CollectibleView
{
private:
    QPixmap chestTexture;
    QRectF bounds; // La hitbox du coffre sur la carte

public:
    CollectibleView();

    // Pour définir où se trouve le coffre et quelle taille il fait
    void setBounds(qreal x, qreal y, qreal width, qreal height);

    // Le dessin (on passe 'hasKey' pour savoir si le coffre est fermé ou ouvert)
    void draw(QPainter &painter, bool isChestClosed);
};

#endif // COLLECTIBLEVIEW_H