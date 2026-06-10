#ifndef BLACKSMITHMODEL_H
#define BLACKSMITHMODEL_H

#include "hitzonemodel.h"
#include "playermodel.h"
#include <QPolygon>
#include <QLineF>
#include <QTransform>
#include <QTimer>

class BlacksmithModel : public PlayerModel
{
    Q_OBJECT
public:
    BlacksmithModel();
    void attack1();
    void attack2();

signals:
    void attackPlayer(HitZoneModel attackHitBox, int damage);
};

#endif // BLACKSMITHMODEL_H
