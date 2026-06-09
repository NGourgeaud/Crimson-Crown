#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include <QObject>
#include "bossmodel.h"
#include "enemymodel.h"
#include "projectilemodel.h"
#include "roommodel.h"
#include "playermodel.h"
#include "QPolygon"

class GameModel : public QObject
{
    Q_OBJECT

private:
    QList<RoomModel*> allRooms;
    QList<EnemyModel*> itsEnemies;
    QList<ProjectileModel*> itsProjeciles;
    BossModel * itsBoss;
    int itsScore;
    PlayerModel * itsPlayer;
    RoomModel * activeRoom;
    int currentLevel;

public:
    GameModel();
    ~GameModel();

    void resetGame(HeroType hero);
    void updateLogic();
    const QList<RoomModel*>& getAllRooms() const;
    const RoomModel* getCurrentRoom() const;
    PlayerModel * getItsPlayer() const;
    void spawnEnemies(bool isBoss);

    const QList<EnemyModel*>& getItsEnemies() const;

    void cheatKillEnemies();

    int getItsScore() const;

    bool areAllCombatRoomsCleared() const;

    BossModel * getItsBoss() const;

    void loadLevel(int levelId);
    QList<ProjectileModel *> getItsProjeciles() const;

signals:
    void roomCleared();
    void gameWon(int score);
    void gameOver(int score);
    void attack(QPolygon);

private slots:
    void onEnemyDeath(EnemyModel * model);
    void onBossDeath(BossModel * model);
    void onEnemyAttack(QPolygon hitbox, int damage);
    void onPlayerAttack(QPolygon hitbox, int damage);
    void onNewProjectile(ProjectileModel * projectile);
    void onEnemyProjectile(ProjectileModel * projectile);
    void onplayerProjectile(ProjectileModel * projectile);
};

#endif // GAMEMODEL_H
