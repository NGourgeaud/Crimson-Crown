#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include "roommodel.h"
// #include "blacksmithmodel.h"

class GameModel : public QObject
{
    Q_OBJECT

private:
    RoomModel* currentRoom;
    // BlackSmithModel* itsPlayer;
    int itsScore;


public:
    GameModel();
    ~GameModel();

    void resetGame(); // Méthode pour (re)lancer une partie

    const RoomModel* getCurrentRoom() const;
    // const BlackSmithModel* getItsPlayer() const;

    void updateLogic(); // Méthode appelée à chaque "tick" du jeu
    void goToNextRoom(); // Nouvelle méthode pour gérer la transition

    void spawnEnemiesForCurrentRoom(int count);

signals:
    void roomCleared();

    void gameOver(int score);

    //  autres méthodes
};

#endif // GAMEMODEL_H