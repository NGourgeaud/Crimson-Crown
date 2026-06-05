#ifndef ENEMYVIEW_H
#define ENEMYVIEW_H

#include "enemymodel.h"
#include "qpainter.h"
#include "config.h"
class EnemyView
{
private:
    QPixmap itsTexTure;
    const EnemyModel * itsModel;
public:
    EnemyView();
    void draw(QPainter * painter);
    void setItsTexture(EnemyType type);
};

#endif // ENEMYVIEW_H
