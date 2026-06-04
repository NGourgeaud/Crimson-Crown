#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include <QObject>
#include "bossmodel.h"
#include "enemymodel.h"
#include "roommodel.h"
#include "playermodel.h"

class GameModel : public QObject
{
    Q_OBJECT

private:
    QList<RoomModel*> allRooms;
    QList<EnemyModel*> itsEnemies;
    BossModel * itsBoss;
    int itsScore;
    PlayerModel * itsPlayer;

public:
    GameModel();
    ~GameModel();

    void resetGame(HeroType hero);
    void updateLogic();
    const QList<RoomModel*>& getAllRooms() const;
    const RoomModel* getCurrentRoom() const;
    PlayerModel * getItsPlayer() const;
    void spawnEnemies(bool isBoss);
    void spawnEnemiesInRoom(RoomModel* room, int count);

    // --- LA LIGNE À AJOUTER ---
    const QList<EnemyModel*>& getItsEnemies() const;

    void cheatKillEnemies();

    int getItsScore() const;

signals:
    void roomCleared();
    void gameOver(int score);

private slots:
    void onEnemyDeath(EnemyModel * model);
    void onBossDeath(BossModel * model);
};

#endif // GAMEMODEL_H
