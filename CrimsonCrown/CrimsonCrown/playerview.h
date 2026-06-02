#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include "PlayerModel.h"
#include <QWidget>

class PlayerView : public QWidget
{
    Q_OBJECT
private:
    PlayerModel * itsModel;
public:
    explicit PlayerView(QWidget *parent = nullptr, PlayerModel *model);
    void draw(QPainter * painter);
};

#endif // PLAYERVIEW_H
