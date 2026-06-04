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
    RoomModel * activeRoom;

public:
    GameModel();
    ~GameModel();

    void resetGame(HeroType hero);
    void updateLogic();
    const QList<RoomModel*>& getAllRooms() const;
    const RoomModel* getCurrentRoom() const;
    PlayerModel * getItsPlayer() const;
    void spawnEnemies(bool isBoss);

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
    void onEnemyAttack(QPolygon * hitbox, int damage);
    void onPlayerAttack(QPolygon * hitbox, int damage);
};

#endif // GAMEMODEL_H
