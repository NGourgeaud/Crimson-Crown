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
    QList<RoomModel*> allRooms;
    QList<EnemyModel*> itsEnemies;
    int itsScore;
    PlayerModel * itsPlayer;

public:
    GameModel();
    ~GameModel();

    void resetGame(HeroType hero);
    const QList<RoomModel*>& getAllRooms() const;
    void updateLogic();
    void spawnEnemiesForCurrentRoom(int count);
    const RoomModel* getCurrentRoom() const;
    PlayerModel * getItsPlayer() const;

    // --- LA LIGNE À AJOUTER ---
    const QList<EnemyModel*>& getItsEnemies() const;

    void cheatKillEnemies();

signals:
    void roomCleared();
    void gameOver(int score);
};

#endif // GAMEMODEL_H