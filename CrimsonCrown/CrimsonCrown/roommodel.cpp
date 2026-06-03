#include "roommodel.h"

// Constructeur : par défaut, la salle n'est pas verrouillée et ne contient aucun ennemi
RoomModel::RoomModel(bool corridor, bool bossRoom)
    : isLocked(false), remainingEnemyCount(0), isCorridor(corridor), isBossRoom(bossRoom)
{
}

// Destructeur : nettoyage de la mémoire pour chaque obstacle instancié
RoomModel::~RoomModel()
{
    qDeleteAll(itsObstacles);
    itsObstacles.clear();
}

// Verrouille les portes
void RoomModel::lockDoors()
{
    isLocked = true;
}

// Déverrouille les portes
void RoomModel::unlockDoors()
{
    isLocked = false;
}

// Démarre une vague d'ennemis
void RoomModel::spawnEnemies(int count)
{
    remainingEnemyCount = count;
    // plus tard EnemyModel ect
}

// Retourne la liste des obstacles
const QList<ObstacleModel*>& RoomModel::getItsObstacles() const
{
    return itsObstacles;
}

// Retourne l'état des portes
bool RoomModel::getIsLocked() const
{
    return isLocked;
}

bool RoomModel::getIsCorridor() const
{
    return isCorridor;
}

bool RoomModel::getIsBossRoom() const
{
    return isBossRoom;
}


// Retourne le nombre d'ennemis restants
int RoomModel::getRemainingEnemyCount() const
{
    return remainingEnemyCount;
}

// Ajoute un nouvel obstacle dans la salle
void RoomModel::addObstacle(int x, int y, int width, int height)
{
    itsObstacles.append(new ObstacleModel(x, y, width, height));
}

