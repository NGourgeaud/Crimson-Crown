#ifndef BOSSVIEW_H
#define BOSSVIEW_H

#include "config.h"
#include "bossmodel.h"
#include "qpixmap.h"
class BossView
{
private:
    QPixmap itsTexTure;
public:
    BossView();
    void draw(QPainter * painter, const BossModel *model);
    void setItsTexture(BossType type);
};

#endif // BOSSVIEW_H
