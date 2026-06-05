#ifndef BOSSVIEW_H
#define BOSSVIEW_H

#include "config.h"
#include "bossmodel.h"
#include "qpixmap.h"
class BossView
{
private:
    QPixmap itsTexTure;
    BossType itsType;
public:
    BossView(BossType type);
    void draw(QPainter * painter, const BossModel *model);
    void setItsTexture();
};

#endif // BOSSVIEW_H
