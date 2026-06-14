// File Name: gamemodel.h
// Group: A11
// Description: Header file for the core game engine model, managing the map, entities, collisions, and game logic.

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
    /// \brief Constructor initializing the game model and its variables.
    GameModel();

    /// \brief Destructor handling the cleanup of dynamically allocated objects.
    ~GameModel();

    /// \brief Full reset of the game state and generation of the first level.
    void resetGame(HeroType hero);

    /// \brief Processes all physical logic, AI, and collisions.
    void updateLogic();

    /// \brief Retrieves the list of all generated rooms.
    const QList<RoomModel*>& getAllRooms() const;

    /// \brief Retrieves the room the player is currently in.
    const RoomModel* getCurrentRoom() const;

    /// \brief Retrieves the main player entity.
    PlayerModel * getItsPlayer() const;

    /// \brief Spawns enemies or a boss into the active room.
    void spawnEnemies(bool isBoss);

    /// \brief Retrieves the list of active standard enemies.
    const QList<EnemyModel*>& getItsEnemies() const;

    /// \brief Handles collision logic for standard enemies.
    void collisionEnemy(EnemyModel * enemy);

    /// \brief Handles collision logic for the boss.
    void collisionBoss(BossModel * enemy);

    /// \brief Retrieves the player's current score.
    int getItsScore() const;

    /// \brief Retrieves the current floor/level ID.
    int getCurrentLevel() const;

    /// \brief Retrieves the player's accumulated gold.
    int getGoldAmount() const;

    /// \brief Checks if all combat rooms in the level are cleared.
    bool areAllCombatRoomsCleared() const;

    /// \brief Retrieves the currently active boss.
    BossModel * getItsBoss() const;

    /// \brief Loads the map architecture based on the level ID.
    void loadLevel(int levelId);

    /// \brief Retrieves the list of active projectiles.
    QList<ProjectileModel *> getItsProjeciles() const;

    /// \brief Retrieves the list of active collectible items.
    QList<ItemModel *> getItsItems() const;

    /// \brief Deducts gold from the balance if affordable.
    bool spendGold(int amount);

signals:
    /// \brief Emitted when a room's enemies are entirely cleared.
    void roomCleared();

    /// \brief Emitted upon game victory.
    void gameWon(int score, HeroType hero);

    /// \brief Emitted upon game defeat.
    void gameOver(int score);

    /// \brief Emitted to visualize an attack hit zone.
    void attack(HitZoneModel);

    /// \brief Emitted to display an informative popup to the player.
    void showPopup(QString message);

    /// \brief Emitted when the boss is successfully spawned.
    void bossSpawned();

    /// \brief Emitted when the boss is defeated.
    void bossKilled();

    /// \brief Emitted when a level transition is triggered.
    void levelCompleted(int nextLevelId);

private slots:
    /// \brief Handles standard enemy death events.
    void onEnemyDeath(EnemyModel * model);

    /// \brief Handles boss death events.
    void onBossDeath(BossModel * model);

    /// \brief Processes enemy melee attacks against the player.
    void onEnemyAttack(HitZoneModel hitbox, int damage);

    /// \brief Processes player attacks against enemies.
    void onPlayerAttack(HitZoneModel hitbox, int damage);

    /// \brief Registers a newly spawned projectile.
    void onNewProjectile(ProjectileModel * projectile);

    /// \brief Updates trajectory and collision for enemy projectiles.
    void onEnemyProjectile(ProjectileModel * projectile);

    /// \brief Updates trajectory and collision for player projectiles.
    void onplayerProjectile(ProjectileModel * projectile);

    /// \brief Triggers the specific minion summoning ability of the King.
    void onKingSummon();
};

#endif // GAMEMODEL_H
