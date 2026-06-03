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
    RoomModel* currentRoom;
    QList<EnemyModel*> itsEnemies;
    int itsScore;
    PlayerModel * itsPlayer;

    int roomCount;
    bool nextisCorridor;

public:
    GameModel();
    ~GameModel();

    void resetGame(HeroType hero); // Méthode pour (re)lancer une partie

    const RoomModel* getCurrentRoom() const;

    void updateLogic(); // Méthode appelée à chaque "tick" du jeu
    void goToNextRoom(); // Nouvelle méthode pour gérer la transition

    void spawnEnemiesForCurrentRoom(int count);

    PlayerModel * getItsPlayer() const;

    int getItsScore() const;

signals:
    void roomCleared();
    void gameOver(int score);
    void gameWon(int finalScore);
};

#endif // GAMEMODEL_H
