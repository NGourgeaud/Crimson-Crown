#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include <QObject>
#include "bossmodel.h"
#include "enemymodel.h"
#include "hitzonemodel.h"
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
    int minEnemy;
    int maxEnemy;
    float statIncreass;
    bool isEasterEggFound;
    bool isHeart1Collected;
    bool isHeart2Collected;

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
    int getCurrentLevel() const;

    bool areAllCombatRoomsCleared() const;

    BossModel * getItsBoss() const;

    void loadLevel(int levelId);
    QList<ProjectileModel *> getItsProjeciles() const;

    bool getIsHeart1Collected() const { return isHeart1Collected; }
    bool getIsHeart2Collected() const { return isHeart2Collected; }

signals:
    void roomCleared();
    void gameWon(int score, HeroType hero);
    void gameOver(int score);
    void attack(HitZoneModel);

private slots:
    void onEnemyDeath(EnemyModel * model);
    void onBossDeath(BossModel * model);
    void onEnemyAttack(HitZoneModel hitbox, int damage);
    void onPlayerAttack(HitZoneModel hitbox, int damage);
    void onNewProjectile(ProjectileModel * projectile);
    void onEnemyProjectile(ProjectileModel * projectile);
    void onplayerProjectile(ProjectileModel * projectile);
    void onKingSummon();
};

#endif // GAMEMODEL_H
