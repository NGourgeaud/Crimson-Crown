#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include "playermodel.h"
#include <QWidget>

class PlayerView : public QWidget
{
    Q_OBJECT
private:
    PlayerModel * itsModel;
    QPixmap itsTexture;
public:
    explicit PlayerView(QWidget * parent, PlayerModel * aModel);
    void draw(QPainter *painter);
    void setItsTexture(std::string namePlayer);
};

#endif // PLAYERVIEW_H
