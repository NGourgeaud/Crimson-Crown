#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include <QObject>
#include "bossmodel.h"
#include "enemymodel.h"
#include "goldmodel.h"
#include "hitzonemodel.h"
#include "itemmodel.h"
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
    QList<ItemModel*> itsItems;
    BossModel * itsBoss;
    int itsScore;
    PlayerModel * itsPlayer;
    RoomModel * activeRoom;
    GoldModel * itsGoldModel;
    int currentLevel;
    int minEnemy;
    int maxEnemy;
    int itsDropChance;
    int itsGoldWorth;
    float statIncreass;
    bool isEasterEggFound;

    bool isHeart1Collected;
    bool isHeart2Collected;
    bool isLevel3BonusCollected;

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

    void collisionEnemy(EnemyModel * enemy);
    void collisionBoss(BossModel * enemy);

    int getItsScore() const;
    int getCurrentLevel() const;
    int getGoldAmount() const;

    bool areAllCombatRoomsCleared() const;

    BossModel * getItsBoss() const;

    void loadLevel(int levelId);
    QList<ProjectileModel *> getItsProjeciles() const;
    QList<ItemModel *> getItsItems() const;

signals:
    void roomCleared();
    void gameWon(int score, HeroType hero);
    void gameOver(int score);
    void attack(HitZoneModel);

    void showPopup(QString message);
    void bossSpawned();
    void bossKilled();

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
