#ifndef ENEMYVIEW_H
#define ENEMYVIEW_H

#include "enemymodel.h"
#include "qpainter.h"
#include "config.h"
class EnemyView
{
private:
    QPixmap itsTexTure;
public:
    EnemyView();
    void draw(QPainter * painter, EnemyModel * model);
    void setItsTexture(EnemyType type);
};

#endif // ENEMYVIEW_H
