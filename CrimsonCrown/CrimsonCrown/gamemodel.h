#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include <QObject>
#include "enemymodel.h"
#include "roommodel.h"
#include "playermodel.h"

class GameModel : public QObject
{
    Q_OBJECT

private:
    QList<RoomModel*> allRooms; // Remplace "currentRoom". Contient toute la carte.
    QList<EnemyModel*> itsEnemies;
    int itsScore;
    PlayerModel * itsPlayer;

public:
    GameModel();
    ~GameModel();

    void resetGame(HeroType hero); // Méthode pour (re)lancer une partie

    // Getter pour récupérer toute la carte d'un coup
    const QList<RoomModel*>& getAllRooms() const;

    void updateLogic(); // Méthode appelée à chaque "tick" du jeu

    void spawnEnemiesForCurrentRoom(int count);

    PlayerModel * getItsPlayer() const;

    int getItsScore() const;

signals:
    void roomCleared();
    void gameOver(int score);
};

#endif // GAMEMODEL_H
