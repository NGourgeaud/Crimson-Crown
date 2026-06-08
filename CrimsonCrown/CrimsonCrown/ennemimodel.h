#ifndef ENNEMIMODEL_H
#define ENNEMIMODEL_H

#include "entitymodel.h"

class EnnemiModel : public EntityModel
{
public:
    EnnemiModel();
    virtual void attack() = 0;
};

#endif // ENNEMIMODEL_H
