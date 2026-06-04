#include "entitymodel.h"

int EntityModel::getItsHP() const
{
    return itsHP;
}

EntityModel::EntityModel() {}

// FIX : On le définit, même s'il est vide
EntityModel::~EntityModel() {}
