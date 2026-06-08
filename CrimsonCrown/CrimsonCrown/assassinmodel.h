#ifndef ASSASSINMODEL_H
#define ASSASSINMODEL_H

#include "playermodel.h"
#include <QPolygon>
#include <QLineF>
#include <QTransform>

class AssassinModel : public PlayerModel
{
    Q_OBJECT
public:
    AssassinModel();
    void attack1() override;
    void attack2() override;

signals:
    void attackPlayer(QPolygon attackHitBox, int damage);
};

#endif // ASSASSINMODEL_H