#ifndef BLACKSMITHMODEL_H
#define BLACKSMITHMODEL_H

#include "playermodel.h"
#include <QPolygon>
#include <QLineF>
#include <QTransform>
#include <QTimer>

class BlacksmithModel : public PlayerModel
{
public:
    BlacksmithModel();
    void attack1();
    void attack2();
signals:
    void attackPlayer(QPolygon attackHitBox, int damage);
};

#endif // BLACKSMITHMODEL_H
