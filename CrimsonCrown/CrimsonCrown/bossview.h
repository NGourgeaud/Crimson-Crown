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
    const BossModel * itsModel;
public:
    BossView(BossType type, const BossModel * model);
    void draw(QPainter * painter);
    void setItsTexture();
};

#endif // BOSSVIEW_H
