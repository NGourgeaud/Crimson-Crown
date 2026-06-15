// File Name: gamemodel.cpp
// Group: A11
// Description: The core engine of the game, managing map generation, room progression, collision physics, entities, and the main game loop.

#include "gamemodel.h"
#include "archermodel.h"
#include "blacksmithmodel.h"
#include "assassinmodel.h"
#include "clergymodel.h"
#include "kingmodel.h"
#include "knightmodel.h"
#include "lancermodel.h"
#include "lordmodel.h"
#include "config.h"
#include "qpainterpath.h"
#include <QRandomGenerator>
#include <QDebug>
#include "huntermodel.h"

///
/// \brief Retrieves the current active boss entity.
/// \return Pointer to the BossModel, or nullptr if no boss is active.
///
BossModel *GameModel::getItsBoss() const
{
    return itsBoss;
}

///
/// \brief Retrieves the list of all active projectiles in the game.
/// \return A QList containing pointers to all ProjectileModels.
///
QList<ProjectileModel *> GameModel::getItsProjeciles() const
{
    return itsProjeciles;
}

///
/// \brief Retrieves the list of all active items (loot, health, gold) in the game.
/// \return A QList containing pointers to all ItemModels.
///
QList<ItemModel *> GameModel::getItsItems() const
{
    return itsItems;
}

///
/// \brief Initializes the game model, default states, and internal variables.
///
GameModel::GameModel() : itsScore(0), itsPlayer(nullptr)
{
    currentLevel = 1;
    itsBoss = nullptr;
    activeRoom = nullptr;
    isEasterEggFound = false;
    itsGoldModel = new GoldModel;
    resetGame(HeroType::None);
}

///
/// \brief Destructor handling the cleanup of dynamically allocated memory for rooms and entities.
///
GameModel::~GameModel()
{
    qDeleteAll(allRooms);
    allRooms.clear();
    if (itsPlayer != nullptr) {
        delete itsPlayer;
    }
}

///
/// \brief Full reset of the game state, cleaning up old entities and regenerating the first level.
/// \param hero The class of the hero selected by the player.
///
void GameModel::resetGame(HeroType hero)
{
    itsGoldModel->resetGold();

    qDeleteAll(allRooms);
    allRooms.clear();

    qDeleteAll(itsEnemies);
    itsEnemies.clear();

    qDeleteAll(itsProjeciles);
    itsProjeciles.clear();

    qDeleteAll(itsItems);
    itsItems.clear();

    if (itsBoss != nullptr) {
        delete itsBoss;
        itsBoss = nullptr;
    }

    if (itsPlayer != nullptr) {
        delete itsPlayer;
        itsPlayer = nullptr;
    }

    // Instantiates the requested player character and links attack/projectile signals
    switch(hero) {
    case HeroType::Blacksmith: {
        BlacksmithModel * newPlayer = new BlacksmithModel();
        itsPlayer = newPlayer;
        connect(newPlayer, &BlacksmithModel::attackPlayer, this, [this](HitZoneModel hitbox, int damage) {this->onPlayerAttack(hitbox, damage);});
        break;
    }
    case HeroType::Assassin: {
        AssassinModel * newPlayer = new AssassinModel();
        itsPlayer = newPlayer;
        connect(newPlayer, &AssassinModel::attackPlayer, this, [this](HitZoneModel hitbox, int damage) {this->onPlayerAttack(hitbox, damage);});
        connect(newPlayer, &AssassinModel::fireProjectile, this, &GameModel::onNewProjectile);
        break;
    };
    case HeroType::Hunter: {
        HunterModel * newPlayer = new HunterModel();
        itsPlayer = newPlayer;
        connect(newPlayer, &HunterModel::attackPlayer, this, [this](HitZoneModel hitbox, int damage) {this->onPlayerAttack(hitbox, damage);});
        connect(newPlayer, &HunterModel::fireProjectile, this, &GameModel::onNewProjectile);
        break;
    }
    default:
        itsPlayer = nullptr;
        break;
    }

    itsScore = 0;
    currentLevel = 1;

    // Loads the actual map architecture for level 1
    loadLevel(currentLevel);
}

///
/// \brief Retrieves the list of all rooms constituting the current level layout.
/// \return A constant reference to the QList of RoomModels.
///
const QList<RoomModel*>& GameModel::getAllRooms() const { return allRooms; }

///
/// \brief Retrieves the list of all active enemies on the map.
/// \return A constant reference to the QList of EnemyModels.
///
const QList<EnemyModel*>& GameModel::getItsEnemies() const { return itsEnemies; }

///
/// \brief Retrieves the main player entity.
/// \return Pointer to the PlayerModel.
///
PlayerModel *GameModel::getItsPlayer() const { return itsPlayer; }

///
/// \brief Generates the dungeon architecture, room placements, and obstacles based on the requested level ID.
/// \param levelId The numerical identifier of the level to load (1, 2, or 3).
///
void GameModel::loadLevel(int levelId)
{
    // 1. Complete cleanup of the previous map
    qDeleteAll(allRooms);
    allRooms.clear();
    qDeleteAll(itsEnemies);
    itsEnemies.clear();
    qDeleteAll(itsItems);
    itsItems.clear();

    qDeleteAll(itsProjeciles);
    itsProjeciles.clear();

    if (itsBoss) {
        delete itsBoss;
        itsBoss = nullptr;
    }
    activeRoom = nullptr;

    // 2. Map generation based on the requested level
    if (levelId == 1) {

        // 1. SPAWN (Far left)
        allRooms.append(new RoomModel(RoomType::Spawn, 0, 0, 600, 600));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 600, 225, 400, 150));

        // 2. COMBAT 1 (Top left)
        allRooms.append(new RoomModel(RoomType::Combat, 1000, -25, 1100, 650));
        allRooms.last()->addObstacle(250, 250, 100, 100);

        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 2100, 225, 400, 150));

        // Corridor going down towards the purple room
        allRooms.append(new RoomModel(RoomType::CorridorVertical, 1475, 625, 150, 400));

        // 3. COMBAT 2 (Central crossroads, framed in purple on the mockup)
        allRooms.append(new RoomModel(RoomType::Combat, 1000, 1025, 1100, 650));

        // 4. LOOT (Bottom branch)
        allRooms.append(new RoomModel(RoomType::CorridorVertical, 1475, 1675, 150, 400));
        allRooms.append(new RoomModel(RoomType::Spawn, 1250, 2075, 600, 600)); // Spawn type used for Loot

        // 5. COMBAT 3 (Right branch)
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 2100, 1275, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, 2500, 1025, 1100, 650));

        // 6. COMBAT 4 (Far right branch)
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 3600, 1275, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, 4000, 1025, 1100, 650));

        // 7. BOSS (Top branch from Combat 3)
        // Corridor going up: Inverted Y position (-400)
        allRooms.append(new RoomModel(RoomType::CorridorVertical, 2975, 625, 150, 400));
        allRooms.append(new RoomModel(RoomType::Boss, 2500, -25, 1100, 650));

        // 8. EXIT (Above the Boss)
        allRooms.append(new RoomModel(RoomType::Exit, 2975, -425, 150, 400));

        // Game tuning stats
        minEnemy = 4;
        maxEnemy = 6;
        statIncreass = 1;
        itsDropChance = 30;
        itsGoldWorth = 3;
    }
    else if (levelId == 2) {
        // --- LEVEL 2 MOCKUP ---
        allRooms.append(new RoomModel(RoomType::Spawn, 0, 0, 600, 600)); // Spawn on the right
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -400, 225, 400, 150));

        allRooms.append(new RoomModel(RoomType::Combat, -1500, -25, 1100, 650)); // Main center

        // TOP Branch
        allRooms.append(new RoomModel(RoomType::CorridorVertical, -1025, -425, 150, 400));
        allRooms.append(new RoomModel(RoomType::Combat, -1500, -1075, 1100, 650));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -400, -825, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, 0, -1075, 1100, 650));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -1900, -825, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, -3000, -1075, 1100, 650));

        // LEFT Branch (End game boss)
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -1900, 225, 400, 150));
        allRooms.append(new RoomModel(RoomType::Boss, -3000, -25, 1100, 650));

        // EXIT ROOM UNDER THE BOSS OF LVL 2
        allRooms.append(new RoomModel(RoomType::Exit, -2525, 625, 150, 400));

        // BOTTOM Branch
        allRooms.append(new RoomModel(RoomType::CorridorVertical, -1025, 625, 150, 400));
        allRooms.append(new RoomModel(RoomType::Combat, -1500, 1025, 1100, 650));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -400, 1275, 400, 150));
        allRooms.append(new RoomModel(RoomType::Spawn, 0, 1075, 600, 600)); // Right Loot zone

        allRooms.append(new RoomModel(RoomType::CorridorVertical, -1025, 1675, 150, 400));
        allRooms.append(new RoomModel(RoomType::Combat, -1500, 2075, 1100, 650));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -1900, 2325, 400, 150));
        allRooms.append(new RoomModel(RoomType::Spawn, -2500, 2100, 600, 600)); // Left Loot zone
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -2900, 2325, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, -4000, 2075, 1100, 650));

        // Pre-placed hearts
        itsItems.append(new ItemModel(QPointF(275, 1350), HEART));
        itsItems.append(new ItemModel(QPointF(-2225, 2375), HEART));

        // Modifying difficulty variables
        minEnemy = 5;
        maxEnemy = 7;
        statIncreass=1.1;
        itsDropChance *= 1.4;
    }
    else if (levelId == 3) {
        // --- LEVEL 3 (Labyrinth) ---
        // 1. SPAWN (Top center)
        allRooms.append(new RoomModel(RoomType::Spawn, 2000, 0, 600, 600));
        allRooms.append(new RoomModel(RoomType::CorridorVertical, 2225, 600, 150, 400));

        // 2. CENTRAL COMBAT (Masterpiece under spawn)
        allRooms.append(new RoomModel(RoomType::Combat, 1750, 1000, 1100, 650));

        // ==========================================
        // RIGHT BRANCHES: LOOT & INVISIBLE CORRIDORS
        // ==========================================
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 2850, 1250, 400, 150));
        allRooms.append(new RoomModel(RoomType::Spawn, 3250, 1025, 600, 600)); // Visible loot

        // --- INVISIBLE PART ---
        // Invisible vertical corridor downwards
        allRooms.append(new RoomModel(RoomType::CorridorVertical, 3475, 1625, 150, 400));
        // Invisible Loot room containing the +10% damage bonus
        allRooms.append(new RoomModel(RoomType::Spawn, 3250, 2025, 600, 600));

        // ==========================================
        // CENTRAL TRUNK AND LOWER RIGHT BRANCH
        // ==========================================
        // Long corridor going down towards the boss
        allRooms.append(new RoomModel(RoomType::CorridorVertical, 2225, 1650, 150, 1400));

        // Long horizontal corridor to the right combat room
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 2375, 2700, 1600, 150));
        allRooms.append(new RoomModel(RoomType::Combat, 3975, 2450, 1100, 650));

        // ==========================================
        // LEFT LABYRINTH (COMBAT SNAKE)
        // ==========================================
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 1350, 1250, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, 250, 1025, 1100, 650)); // Left Combat 1

        allRooms.append(new RoomModel(RoomType::CorridorVertical, 725, 1675, 150, 400));
        allRooms.append(new RoomModel(RoomType::Combat, 250, 2075, 1100, 650)); // Left Combat 2

        allRooms.append(new RoomModel(RoomType::CorridorVertical, 725, 2725, 150, 400));
        allRooms.append(new RoomModel(RoomType::Combat, 250, 3125, 1100, 650)); // Left Combat 3

        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -150, 3375, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, -1250, 3125, 1100, 650)); // Left Combat 4

        allRooms.append(new RoomModel(RoomType::CorridorVertical, -775, 3775, 150, 400));
        allRooms.append(new RoomModel(RoomType::Spawn, -1000, 4175, 600, 600)); // Bottom left Loot

        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -400, 4400, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, 0, 4150, 1100, 650)); // Left Combat 5

        // ==========================================
        // DUNGEON END: BOSS & FINAL EXIT
        // ==========================================
        allRooms.append(new RoomModel(RoomType::Boss, 1750, 3050, 1100, 650));
        allRooms.append(new RoomModel(RoomType::Exit, 2225, 3700, 150, 400));

        // Adding pre-placed hearts in the visible loot rooms
        itsItems.append(new ItemModel(QPointF(3550, 1325), HEART));
        itsItems.append(new ItemModel(QPointF(-700, 4475), HEART));

        // Modifying difficulty variables
        minEnemy = 6;
        maxEnemy = 8;
        statIncreass=1.2;
        itsDropChance *= 1.4;
    }

    // ==========================================
    // PROCEDURAL OBSTACLE GENERATION
    // ==========================================
    std::random_device rd;
    std::default_random_engine eng(rd());

    for (RoomModel* room : std::as_const(allRooms)) {
        // We only target Combat rooms (not Spawns, Corridors, or Boss to avoid trapping him)
        if (room->getType() == RoomType::Combat) {

            int w = room->getWidth();
            int h = room->getHeight();

            // 1. Pick a random number of obstacles per room (between 2 and 5)
            std::uniform_int_distribution<int> nbObstaclesDist(2, 5);
            int nbObstacles = nbObstaclesDist(eng);

            // 2. Define a "safe zone" to avoid blocking doors
            // Leaving a 150 pixels margin from the walls
            std::uniform_int_distribution<int> posX(150, w - 200);
            std::uniform_int_distribution<int> posY(150, h - 200);

            // 3. Define a random size for each obstacle (between 40 and 80 pixels)
            std::uniform_int_distribution<int> sizeDist(40, 80);

            // Creation and appending of obstacles
            for (int i = 0; i < nbObstacles; ++i) {
                int obsSize = sizeDist(eng);
                int x = posX(eng);
                int y = posY(eng);

                room->addObstacle(x, y, obsSize, obsSize);
            }
        }
    }

    // 3. Reposition the character to the center of the current Spawn zone
    if (itsPlayer != nullptr) {
        for (RoomModel* room : std::as_const(allRooms)) {
            if (room->getType() == RoomType::Spawn) {
                itsPlayer->move(room->getBounds().center());
                break;
            }
        }
    }
}

///
/// \brief Populates the active room with enemies or the level's specific boss.
/// \param isBoss Boolean indicating whether to spawn standard mobs or the boss entity.
///
void GameModel::spawnEnemies(bool isBoss)
{
    QRect roomBoundaries = activeRoom->getBounds().toRect();
    if (!isBoss){
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> RoomX(roomBoundaries.topLeft().rx()+60,roomBoundaries.topRight().rx()-60);
        std::uniform_int_distribution<int> RoomY(roomBoundaries.topLeft().ry()+60,roomBoundaries.bottomLeft().ry()-60);
        std::uniform_int_distribution<int> Type(LANCER,ARCHER);

        std::uniform_int_distribution<int> EnemiesNb(minEnemy,maxEnemy);

        int nbEnemies = EnemiesNb(eng);
        for (int i = 0; i < nbEnemies; i++){
            switch (Type(eng)) {
            case LANCER:
            {
                LancerModel * newEnnemy = new LancerModel(LancerDefaultMaxHp*statIncreass, LancerDefaultSpeed, RoomX(eng), RoomY(eng), LancerDefaultAttackCoolDown, LancerDefaultAttackDamage*statIncreass);
                itsEnemies.append(newEnnemy);

                // Using a lambda to secure parameter passing
                connect(newEnnemy, &LancerModel::death, this, [this, newEnnemy]() {this->onEnemyDeath(newEnnemy);});
                connect(newEnnemy, &LancerModel::attacking, this, [this](HitZoneModel hitbox, int damage) {this->onEnemyAttack(hitbox, damage);});
                break;
            }
            case KNIGHT:
            {
                KnightModel * newEnnemy = new KnightModel(KnightDefaultMaxHp, KnightDefaultSpeed, RoomX(eng), RoomY(eng), KnightDefaultAttackCoolDown, KnightDefaultAttackDamage);
                itsEnemies.append(newEnnemy);

                connect(newEnnemy, &KnightModel::death, this, [this, newEnnemy]() {this->onEnemyDeath(newEnnemy);});
                connect(newEnnemy, &KnightModel::attacking, this, [this](HitZoneModel hitbox, int damage) {this->onEnemyAttack(hitbox, damage);});
                break;
            }
            case ARCHER:
            {
                ArcherModel * newEnnemy = new ArcherModel(ArcherDefaultMaxHp, ArcherDefaultSpeed, RoomX(eng), RoomY(eng), ArcherDefaultAttackCoolDown, ArcherDefaultAttackDamage);
                itsEnemies.append(newEnnemy);

                connect(newEnnemy, &ArcherModel::death, this, [this, newEnnemy]() {this->onEnemyDeath(newEnnemy);});
                connect(newEnnemy, &ArcherModel::attacking, this, &GameModel::onNewProjectile);
                break;
            }
            default:
                break;
            }
        }
    }
    else {
        // Method to spawn the boss based on the current level
        switch (currentLevel) {
        case 1:
        {
            clergyModel * newClergy = new clergyModel(ClergyDefaultMaxHp, ClergyDefaultSpeed, roomBoundaries.center().rx(), roomBoundaries.center().ry(),
                                                     ClergyDefaultAttack1CoolDown, ClergyDefaultAttack2CoolDown, ClergyDefaultAttack1Damage, ClergyDefaultAttack2Damage);
            itsBoss = newClergy;

            connect(newClergy, &clergyModel::death, this, &GameModel::onBossDeath);
            connect(newClergy, &clergyModel::attacking, this, &GameModel::onNewProjectile);
            break;
        }
        case 2:
        {
            LordModel * newLord = new LordModel(LordDefaultMaxHp, LordDefaultSpeed, roomBoundaries.center().rx(), roomBoundaries.center().ry(),
                                               LordDefaultAttack1CoolDown, LordDefaultAttack2CoolDown, LordDefaultAttack1Damage, LordDefaultAttack2Damage);
            itsBoss = newLord;

            emit bossSpawned();

            connect(newLord, &LordModel::death, this, &GameModel::onBossDeath);
            connect(newLord, &LordModel::attacking, this, [this](HitZoneModel hitbox, int damage) {this->onEnemyAttack(hitbox, damage);});
            break;
        }
        case 3:
        {
            KingModel * newKing = new KingModel(KingDefaultMaxHp, KingDefaultSpeed, roomBoundaries.center().rx(), roomBoundaries.center().ry(),
                                               KingDefaultAttack1CoolDown, KingDefaultAttack2CoolDown, KingDefaultAttack1Damage, KingDefaultAttack2Damage);
            itsBoss = newKing;

            connect(newKing, &KingModel::death, this, &GameModel::onBossDeath);
            connect(newKing, &KingModel::attacking, [this](HitZoneModel hitbox, int damage) {this->onEnemyAttack(hitbox, damage);});
            connect(newKing, &KingModel::spawnEnemies, this, &GameModel::onKingSummon);
            break;
        }
        default:
            break;
        }
    }
}

///
/// \brief Processes all physical logic, AI movements, collisions, item pickups, and triggers map progression.
///
void GameModel::updateLogic()
{
    // 1. AI UPDATE
    if (itsPlayer != nullptr) {
        bool isPlayerInvisible = false;
        AssassinModel* assassin = dynamic_cast<AssassinModel*>(itsPlayer);
        if (assassin && assassin->getIsInvisible()) {
            isPlayerInvisible = true;
        }

        // Only update enemy chasing if the player is not currently invisible
        if (!isPlayerInvisible) {
            // A. Enemies Update
            for (int i = itsEnemies.size() - 1; i >= 0; --i) {
                EnemyModel * enemy = itsEnemies.at(i);
                collisionEnemy(enemy);
            }

            if(itsBoss != nullptr){
                collisionBoss(itsBoss);
            }
        }
    }

    // Projectile update
    if (!itsProjeciles.empty()){
        for (ProjectileModel * pro : std::as_const(itsProjeciles)){
            pro->update();
        }
    }

    // 2. PLAYER UPDATE & COLLISIONS
    if (itsPlayer != nullptr) {
        QPointF oldPos = itsPlayer->getHitBox().topLeft();
        itsPlayer->update();

        // --- A. DETECTION OF THE ACTIVE ROOM ---
        for (RoomModel* room : std::as_const(allRooms)) {
            if (room->getBounds().contains(itsPlayer->getHitBox().center())) {
                activeRoom = room;
                break;
            }
        }

        // --- B. DISCOVERY OF A NEW ROOM AND SPAWN OF ENEMIES ---
        if (activeRoom != nullptr) {
            RoomType type = activeRoom->getType();

            // =========================================================
            // EXIT CORRIDOR MANAGEMENT (LEVEL PROGRESSION)
            // =========================================================
            if (type == RoomType::Exit) {
                if (currentLevel == 1) {
                    currentLevel = 2;
                    emit levelCompleted(currentLevel); // Notifies the controller
                    return;
                }
                else if (currentLevel == 2) {
                    currentLevel = 3;
                    emit levelCompleted(currentLevel);
                    return;
                }
                else if (currentLevel == 3) {
                    emit gameWon(itsScore, itsPlayer->getItsName());
                    return;
                }
            }

            // --- Classic combat logic if the room hasn't been visited yet ---
            if (!activeRoom->getIsVisited() && activeRoom->getBounds().contains(itsPlayer->getHitBox())){
                activeRoom->setIsVisited();

                if (type == RoomType::Combat) {
                    spawnEnemies(false);
                    itsScore += 100;
                    activeRoom->lockDoors();
                }
                else if (type == RoomType::Boss){
                    spawnEnemies(true);
                    itsScore += 100;
                    activeRoom->lockDoors();
                }
            }
        }

        // ==========================================
        // --- ITEM LOGIC (HEALING & GOLD) ---
        // ==========================================
        if (!itsItems.empty()){
            for (ItemModel * item : std::as_const(itsItems)){
                if (itsPlayer->getHitBox().intersects(item->getItsHitbox())){
                    switch (item->getItsType()) {
                    case HEART:
                        itsPlayer->heal(5);
                        break;
                    case GOLD:
                        itsGoldModel->addGold(itsGoldWorth+itsGoldWorth*(1.4*currentLevel-1));
                    default:
                        break;
                    }
                    itsItems.removeOne(item);
                    delete item;
                }
            }
        }

        // ==========================================
        // --- SECRET BONUS LOGIC (LEVEL 3) ---
        // ==========================================
        if (currentLevel == 3 && !isLevel3BonusCollected) {

            // Center of the invisible room calculated at (3250 + 300, 2025 + 300) = (3550, 2325)
            // 60x60 pixels pickup hitbox anchored in the middle
            QRectF secretBonusZone(3520, 2295, 60, 60);

            if (itsPlayer->getHitBox().intersects(secretBonusZone)) {
                itsPlayer->increaseDamage(0.10); // Permanent +10% damage!
                itsScore += 100;                 // Grant 100 points for discovering the secret room
                isLevel3BonusCollected = true;

                // Triggers the golden screen pop-up
                emit showPopup("BONUS SECRET DETERRE : +10% DEGATS !");
            }
        }

        // ==========================================
        // --- EASTER EGG & SECRET CORRIDOR LOGIC ---
        // ==========================================

        // 1. Detect if the boss is dead
        bool isBossDead = false;
        for (RoomModel* room : std::as_const(allRooms)) {
            if (room->getType() == RoomType::Boss && room->getIsVisited() && itsBoss == nullptr) {
                isBossDead = true;
                break;
            }
        }

        if (currentLevel == 1) {

            if (!isEasterEggFound) {

                // A. Pick up the key
                QRectF secretKeyZone(2430, 275, 50, 50);
                if (isBossDead && !itsPlayer->getHasSecretKey() && itsPlayer->getHitBox().intersects(secretKeyZone)) {
                    itsPlayer->setHasSecretKey(true);
                    qDebug() << "Clef secrete trouvee dans le couloir invisible !";
                }

                // B. Open the chest
                QRectF chestZone(1500, 2325, 100, 100);
                QRectF interactionZone = chestZone.adjusted(-20, -20, 20, 20);

                if (itsPlayer->getHasSecretKey() && itsPlayer->getHitBox().intersects(interactionZone)) {
                    qDebug() << "COFFRE OUVERT ! EASTER EGG DECOUVERT !";
                    itsPlayer->setHasSecretKey(false);
                    itsScore += 200;

                    // LOCKS THE EASTER EGG FOR THE REST OF THE GAME
                    isEasterEggFound = true;
                }
            }
        }

        // --- C. WALL MANAGEMENT (UPDATED FOR INVISIBLE CORRIDOR) ---
        bool collision = false;
        bool bossAccess = areAllCombatRoomsCleared();

        QPainterPath mapBounds;
        for (RoomModel* room : std::as_const(allRooms)) {
            if (room->getType() == RoomType::Boss && !bossAccess) {
                continue;
            }

            // --- SECRET COLLISION MODIFICATION ---
            // If it's our secret corridor and the boss isn't dead, we don't add it to the walkable bounds
            if (room->getOffsetX() == 2100 && room->getOffsetY() == 225 && !isBossDead) {
                continue;
            }

            mapBounds.addRect(room->getOffsetX(), room->getOffsetY(),
                              room->getWidth(), room->getHeight());
        }
        mapBounds = mapBounds.simplified();

        // 1. Detection of map borders and closed doors
        if (activeRoom != nullptr && activeRoom->getIsLocked()) {
            if (!activeRoom->getBounds().contains(itsPlayer->getHitBox())) {
                collision = true;
            }
        }
        else {
            if (!mapBounds.contains(itsPlayer->getHitBox())) {
                collision = true;
            }
        }

        // --- D. OBSTACLE COLLISIONS (Barrels, etc.) ---
        for (RoomModel* room : std::as_const(allRooms)) {
            for (ObstacleModel* obs : room->getItsObstacles()) {
                QRectF globalObsRect = obs->getHitBox();
                globalObsRect.translate(room->getBounds().x(), room->getBounds().y());

                if (itsPlayer->getHitBox().intersects(globalObsRect)) {
                    collision = true;
                }
            }
        }

        // --- E. PHYSICS APPLICATION ---
        if (collision) {
            itsPlayer->move(oldPos);
        }
    }
}

///
/// \brief Handles movement logic, wall sliding, and collision detection for standard enemies.
/// \param enemy Pointer to the EnemyModel currently being processed.
///
void GameModel::collisionEnemy(EnemyModel * enemy)
{
    bool isCollision = false;

    // 1. AI UPDATE (The enemy calculates its trajectory and moves towards the player first)
    enemy->update(itsPlayer->getHitBox().center());

    // 2. Collisions with other enemies
    for (int j = 0; j < itsEnemies.size(); ++j) {
        EnemyModel * otherEnemy = itsEnemies.at(j);

        // We ignore collision with itself
        if (enemy == otherEnemy) {
            continue;
        }

        QRectF teamCollision = enemy->getHitBox().intersected(otherEnemy->getHitBox());

        // If the two enemies crash into each other
        if (!teamCollision.isEmpty()) {

            // We retrieve the current position to correct it
            QPointF newPos = enemy->getHitBox().topLeft();
            QRectF otherRect = otherEnemy->getHitBox();

            // CASE 1: Top or bottom collision (Vertical push)
            if (teamCollision.width() > teamCollision.height()) {
                if(enemy->getHitBox().center().y() < otherRect.center().y()){
                    newPos.setY(otherRect.top() - enemy->getHitBox().height());
                }
                else{
                    newPos.setY(otherRect.bottom());
                }
            }
            // CASE 2: Side collision (Horizontal push)
            else if (teamCollision.height() > teamCollision.width()) {
                if(enemy->getHitBox().center().x() < otherRect.center().x()){
                    newPos.setX(otherRect.left() - enemy->getHitBox().width());
                }
                else{
                    newPos.setX(otherRect.right());
                }
            }
            // CASE 3: Corner collision
            else {
                if(enemy->getHitBox().center().y() < otherRect.center().y()){
                    newPos.setY(otherRect.top() - enemy->getHitBox().height());
                }
                else{
                    newPos.setY(otherRect.bottom());
                }

                if(enemy->getHitBox().center().x() < otherRect.center().x()){
                    newPos.setX(otherRect.left() - enemy->getHitBox().width());
                }
                else{
                    newPos.setX(otherRect.right());
                }
            }

            // We apply the repulsion so they don't overlap
            enemy->move(newPos);
            isCollision = true;
        }
    }

    // 3. Collision with map borders
    QPointF newPosMap = enemy->getHitBox().topLeft();
    QRectF roomBounds = activeRoom->getBounds();

    if (newPosMap.x() < roomBounds.left()) {
        newPosMap.setX(roomBounds.left());
    }
    else if (newPosMap.x() + enemy->getHitBox().width() > roomBounds.right()) {
        newPosMap.setX(roomBounds.right() - enemy->getHitBox().width());
    }

    if (newPosMap.y() < roomBounds.top()) {
        newPosMap.setY(roomBounds.top());
    }
    else if (newPosMap.y() + enemy->getHitBox().height() > roomBounds.bottom()) {
        newPosMap.setY(roomBounds.bottom() - enemy->getHitBox().height());
    }
    enemy->move(newPosMap);

    // 4. Collision with obstacles
    if (!activeRoom->getItsObstacles().empty()) {
        QRectF zoneCollision;
        // Retrieve the absolute position the enemy just calculated
        QPointF newPos = enemy->getHitBox().topLeft();

        for (ObstacleModel * obstacle : activeRoom->getItsObstacles()) {

            // Calculate the Global Hitbox of the obstacle
            QRectF globalObsRect = obstacle->getHitBox();
            globalObsRect.translate(activeRoom->getBounds().x(), activeRoom->getBounds().y());

            zoneCollision = enemy->getHitBox().intersected(globalObsRect);

            // If there is a collision with this obstacle
            if(!zoneCollision.isEmpty()){

                // CASE 1: Top or bottom collision (Horizontal Wall)
                if (zoneCollision.width() > zoneCollision.height()) {
                    // Repulsion against the wall
                    if(enemy->getHitBox().center().y() < globalObsRect.center().y()){
                        newPos.setY(globalObsRect.top() - enemy->getHitBox().height());
                    }
                    else{
                        newPos.setY(globalObsRect.bottom());
                    }

                    // --- SMART SLIDING (NO RESTRICTION + FRICTION) ---
                    // The enemy ALWAYS slides towards the player's X position
                    if (itsPlayer->getHitBox().center().x() > enemy->getHitBox().center().x()) {
                        newPos.setX(newPos.x() + (enemy->getitsSpeed())); // Slides right
                    } else {
                        newPos.setX(newPos.x() - (enemy->getitsSpeed())); // Slides left
                    }
                }
                // CASE 2: Side collision (Vertical Wall)
                else if (zoneCollision.height() > zoneCollision.width()) {
                    // Repulsion against the wall
                    if(enemy->getHitBox().center().x() < globalObsRect.center().x()){
                        newPos.setX(globalObsRect.left() - enemy->getHitBox().width());
                    }
                    else{
                        newPos.setX(globalObsRect.right());
                    }

                    // --- SMART SLIDING (NO RESTRICTION + FRICTION) ---
                    // The enemy ALWAYS slides towards the player's Y position
                    if (itsPlayer->getHitBox().center().y() > enemy->getHitBox().center().y()) {
                        newPos.setY(newPos.y() + (enemy->getitsSpeed())); // Slides down
                    } else {
                        newPos.setY(newPos.y() - (enemy->getitsSpeed())); // Slides up
                    }
                }
                // CASE 3: Corner collision (width == height)
                else {
                    // Simple diagonal repulsion
                    if(enemy->getHitBox().center().y() < globalObsRect.center().y()){
                        newPos.setY(globalObsRect.top() - enemy->getHitBox().height());
                    }
                    else{
                        newPos.setY(globalObsRect.bottom());
                    }

                    if(enemy->getHitBox().center().x() < globalObsRect.center().x()){
                        newPos.setX(globalObsRect.left() - enemy->getHitBox().width());
                    }
                    else{
                        newPos.setX(globalObsRect.right());
                    }
                }

                // APPLY THE MOVEMENT
                enemy->move(newPos);
                isCollision = true;
            }
        }
    }

    // Resume tracking if no hard obstacle collision occurred
    if(!isCollision){
        enemy->update(itsPlayer->getHitBox().center());
    }
}

///
/// \brief Handles movement logic, wall sliding, and collision detection specifically for bosses.
/// \param enemy Pointer to the BossModel currently being processed.
///
void GameModel::collisionBoss(BossModel * enemy)
{
    bool isCollision = false;

    // 2. Collisions with other enemies
    for (int j = 0; j < itsEnemies.size(); ++j) {
        EnemyModel * otherEnemy = itsEnemies.at(j);
        QRectF teamCollision = enemy->getHitBox().intersected(otherEnemy->getHitBox());

        // If the two enemies crash into each other
        if (!teamCollision.isEmpty()) {

            // We retrieve the current position to correct it
            QPointF newPos = enemy->getHitBox().topLeft();
            QRectF otherRect = otherEnemy->getHitBox();

            // CASE 1: Top or bottom collision (Vertical push)
            if (teamCollision.width() > teamCollision.height()) {
                if(enemy->getHitBox().center().y() < otherRect.center().y()){
                    newPos.setY(otherRect.top() - enemy->getHitBox().height());
                }
                else{
                    newPos.setY(otherRect.bottom());
                }
            }
            // CASE 2: Side collision (Horizontal push)
            else if (teamCollision.height() > teamCollision.width()) {
                if(enemy->getHitBox().center().x() < otherRect.center().x()){
                    newPos.setX(otherRect.left() - enemy->getHitBox().width());
                }
                else{
                    newPos.setX(otherRect.right());
                }
            }
            // CASE 3: Corner collision
            else {
                if(enemy->getHitBox().center().y() < otherRect.center().y()){
                    newPos.setY(otherRect.top() - enemy->getHitBox().height());
                }
                else{
                    newPos.setY(otherRect.bottom());
                }

                if(enemy->getHitBox().center().x() < otherRect.center().x()){
                    newPos.setX(otherRect.left() - enemy->getHitBox().width());
                }
                else{
                    newPos.setX(otherRect.right());
                }
            }

            // We apply the repulsion so they don't overlap
            enemy->move(newPos);
            isCollision = true;
        }
    }

    // 3. Collision with map borders
    QPointF newPosMap = enemy->getHitBox().topLeft();
    QRectF roomBounds = activeRoom->getBounds();

    if (newPosMap.x() < roomBounds.left()) {
        newPosMap.setX(roomBounds.left());
    }
    else if (newPosMap.x() + enemy->getHitBox().width() > roomBounds.right()) {
        newPosMap.setX(roomBounds.right() - enemy->getHitBox().width());
    }

    if (newPosMap.y() < roomBounds.top()) {
        newPosMap.setY(roomBounds.top());
    }
    else if (newPosMap.y() + enemy->getHitBox().height() > roomBounds.bottom()) {
        newPosMap.setY(roomBounds.bottom() - enemy->getHitBox().height());
    }
    enemy->move(newPosMap);

    // 4. Collision with obstacles
    if (!activeRoom->getItsObstacles().empty()){

        QRectF zoneCollision;
        QPointF newPos = enemy->getHitBox().topLeft();

        for (ObstacleModel * obstacle : activeRoom->getItsObstacles()) {

            // Calculate the Global Hitbox of the obstacle
            QRectF globalObsRect = obstacle->getHitBox();
            globalObsRect.translate(activeRoom->getBounds().x(), activeRoom->getBounds().y());

            zoneCollision = enemy->getHitBox().intersected(globalObsRect);

            if(!zoneCollision.isEmpty()){

                // CASE 1: Top or bottom collision (Horizontal Wall)
                if (zoneCollision.width() > zoneCollision.height()) {
                    if(enemy->getHitBox().center().y() < globalObsRect.center().y()){
                        newPos.setY(globalObsRect.top() - enemy->getHitBox().height());
                    }
                    else{
                        newPos.setY(globalObsRect.bottom());
                    }

                    // --- SMART SLIDING ---
                    if (itsPlayer->getHitBox().center().x() > enemy->getHitBox().center().x()) {
                        newPos.setX(newPos.x() + (enemy->getItsSpeed()));
                    } else {
                        newPos.setX(newPos.x() - (enemy->getItsSpeed()));
                    }
                }
                // CASE 2: Side collision (Vertical Wall)
                else if (zoneCollision.height() > zoneCollision.width()) {
                    if(enemy->getHitBox().center().x() < globalObsRect.center().x()){
                        newPos.setX(globalObsRect.left() - enemy->getHitBox().width());
                    }
                    else{
                        newPos.setX(globalObsRect.right());
                    }

                    // --- SMART SLIDING ---
                    if (itsPlayer->getHitBox().center().y() > enemy->getHitBox().center().y()) {
                        newPos.setY(newPos.y() + (enemy->getItsSpeed()));
                    } else {
                        newPos.setY(newPos.y() - (enemy->getItsSpeed()));
                    }
                }
                // CASE 3: Corner collision
                else {
                    if(enemy->getHitBox().center().y() < globalObsRect.center().y()){
                        newPos.setY(globalObsRect.top() - enemy->getHitBox().height());
                    }
                    else{
                        newPos.setY(globalObsRect.bottom());
                    }

                    if(enemy->getHitBox().center().x() < globalObsRect.center().x()){
                        newPos.setX(globalObsRect.left() - enemy->getHitBox().width());
                    }
                    else{
                        newPos.setX(globalObsRect.right());
                    }
                }

                enemy->move(newPos);
                isCollision = true;
            }
        }
    }

    if(!isCollision){
        enemy->update(itsPlayer->getHitBox().center());
    }
}

///
/// \brief Retrieves the player's current accumulated score.
/// \return An integer representing the total score.
///
int GameModel::getItsScore() const
{
    return itsScore;
}

///
/// \brief Handles the death sequence of a standard enemy, spawning drops and granting score.
/// \param model Pointer to the EnemyModel that has died.
///
void GameModel::onEnemyDeath(EnemyModel * model)
{
    itsEnemies.removeOne(model);
    itsScore += 10; // +10 Points per standard kill

    if (itsEnemies.empty() && !itsBoss){
        activeRoom->unlockDoors();
    }

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> dropChance(1,100);
    std::uniform_int_distribution<int> dropType(HEART,GOLD);

    if (dropChance(eng) < itsDropChance){
        switch (dropType(eng)) {
        case HEART:
            itsItems.append(new ItemModel(model->getHitBox().center(), HEART));
            break;
        case GOLD:
            itsItems.append(new ItemModel(model->getHitBox().center(), GOLD));
            break;
        default:
            break;
        }
    }

    QObject * qobjEnemy = dynamic_cast<QObject*>(model);
    if (qobjEnemy) {
        qobjEnemy->deleteLater();
    }
    else {
        delete model;
    }

    // specific mechanic for the king boss to summon minions
    if (itsBoss){
        if (itsEnemies.count() < 2){
            KingModel * king = dynamic_cast<KingModel*>(itsBoss);
            king->setCanSummon(true);
        }
    }
}

///
/// \brief Handles the death sequence of the boss, unlocking the room and granting massive score.
/// \param model Pointer to the BossModel that has died.
///
void GameModel::onBossDeath(BossModel * model)
{
    QObject * qobjBoss = dynamic_cast<QObject*>(model);
    if (activeRoom != nullptr && activeRoom->getType() == RoomType::Boss) {
        activeRoom->unlockDoors();
    }
    if (qobjBoss) disconnect(qobjBoss);

    itsBoss = nullptr;
    itsScore += 150;

    emit bossKilled();
    itsPlayer->regen();

    delete model;
}

///
/// \brief Processes an enemy's melee attack, checking if the hitbox intersects the player.
/// \param hitbox The geometric zone representing the attack area.
/// \param damage The amount of damage to deal if a collision occurs.
///
void GameModel::onEnemyAttack(HitZoneModel hitbox, int damage)
{
    emit attack(hitbox);
    if (hitbox.getItsPolygon().intersects(itsPlayer->getHitBox().toRect())){
        itsPlayer->takeDamage(damage);
    }
}

///
/// \brief Processes the player's melee attack, damaging any enemies caught in the hitbox.
/// \param hitbox The geometric zone representing the player's attack area.
/// \param damage The amount of damage the player inflicts.
///
void GameModel::onPlayerAttack(HitZoneModel hitbox, int damage)
{
    emit attack(hitbox);
    if (!itsEnemies.empty()){
        for (EnemyModel* enemy : std::as_const(itsEnemies)){
            if (hitbox.getItsPolygon().intersects(enemy->getHitBox().toRect())){
                enemy->takeDamage(damage);
            }
        }
    }
    else if(itsBoss){
        if (hitbox.getItsPolygon().intersects(itsBoss->getHitBox().toRect())){
            itsBoss->takeDamage(damage);
        }
    }
}

///
/// \brief Registers a newly spawned projectile into the game tracking list.
/// \param projectile Pointer to the new ProjectileModel to be tracked.
///
void GameModel::onNewProjectile(ProjectileModel * projectile)
{
    itsProjeciles.append(projectile);
    if (projectile->getItsTeam() == ENEMY)
        connect(projectile, &ProjectileModel::tick, this, &GameModel::onEnemyProjectile);
    else
        connect(projectile, &ProjectileModel::tick, this, &GameModel::onplayerProjectile);
}

///
/// \brief Processes the trajectory and collision logic for enemy-fired projectiles.
/// \param projectile Pointer to the enemy projectile being updated.
///
void GameModel::onEnemyProjectile(ProjectileModel * projectile)
{
    // Collision with player
    if (projectile->getItsHitbox().intersects(itsPlayer->getHitBox())){
        itsPlayer->takeDamage(projectile->getItsDamage());
        disconnect(projectile);
        itsProjeciles.removeOne(projectile);
        projectile->deleteLater();
        return; // Prevent further logic execution
    }

    if (projectile->getIsInvincible()) {
        return;
    }

    // Collision with obstacles
    if (!activeRoom->getItsObstacles().empty()){
        for (ObstacleModel * obstacle : activeRoom->getItsObstacles()){
            QRectF globalObsRect = obstacle->getHitBox();
            globalObsRect.translate(activeRoom->getBounds().x(), activeRoom->getBounds().y());

            if (projectile->getItsHitbox().intersects(globalObsRect)){
                disconnect(projectile);
                itsProjeciles.removeOne(projectile);
                delete projectile;
                return;
            }
        }
    }

    // Define the inner area (the hollow space past the border)
    QMarginsF margins(1, 1, 1, 1);
    QRectF innerSpace = activeRoom->getBounds().marginsRemoved(margins);

    // If innerSpace fully contains r1, it means r1 safely bypassed the border
    // and is floating in the middle. Otherwise, it's stuck in the border zone.
    if (!innerSpace.contains(projectile->getItsHitbox())){
        disconnect(projectile);
        itsProjeciles.removeOne(projectile);
        delete projectile;
        return;
    }
}

///
/// \brief Processes the trajectory and collision logic for player-fired projectiles.
/// \param projectile Pointer to the player projectile being updated.
///
void GameModel::onplayerProjectile(ProjectileModel *projectile)
{
    // Collision with standard enemies
    if(!itsEnemies.empty()){
        for (EnemyModel * enemy : std::as_const(itsEnemies)){
            if (projectile->getItsHitbox().intersects(enemy->getHitBox())){
                enemy->takeDamage(projectile->getItsDamage());
                disconnect(projectile);
                itsProjeciles.removeOne(projectile);
                delete projectile;
                return;
            }
        }
    }

    // Collision with the boss
    if (itsBoss){
        if (projectile->getItsHitbox().intersects(itsBoss->getHitBox())){
            itsBoss->takeDamage(projectile->getItsDamage());
            disconnect(projectile);
            itsProjeciles.removeOne(projectile);
            delete projectile;
            return;
        }
    }

    // Collision with obstacles
    if (!activeRoom->getItsObstacles().empty()){
        for (ObstacleModel * obstacle : activeRoom->getItsObstacles()){
            QRectF globalObsRect = obstacle->getHitBox();
            globalObsRect.translate(activeRoom->getBounds().x(), activeRoom->getBounds().y());

            if (projectile->getItsHitbox().intersects(globalObsRect)){
                disconnect(projectile);
                itsProjeciles.removeOne(projectile);
                delete projectile;
                return;
            }
        }
    }

    // Define the inner area (the hollow space past the border)
    QMarginsF margins(1, 1, 1, 1);
    QRectF innerSpace = activeRoom->getBounds().marginsRemoved(margins);

    if (!innerSpace.contains(projectile->getItsHitbox())){
        disconnect(projectile);
        itsProjeciles.removeOne(projectile);
        delete projectile;
        return;
    }
}

///
/// \brief Triggers the specific minion summoning ability of the King boss.
///
void GameModel::onKingSummon()
{
    int oldMinEnemy = minEnemy;
    int oldMaxEnemy = maxEnemy;
    minEnemy = 1;
    maxEnemy = 2;
    spawnEnemies(false);
    minEnemy = oldMinEnemy;
    maxEnemy = oldMaxEnemy;

    if (itsEnemies.count() >= 3){
        KingModel * king = dynamic_cast<KingModel*>(itsBoss);
        king->setCanSummon(false);
    }
}

///
/// \brief Checks if all combat rooms in the current level have been successfully cleared.
/// \return True if no combat room is locked or unvisited, false otherwise.
///
bool GameModel::areAllCombatRoomsCleared() const
{
    for (RoomModel* room : allRooms) {
        if (room->getType() == RoomType::Combat) {
            // If a combat room has not yet been visited OR is still locked (combat in progress)
            if (!room->getIsVisited() || room->getIsLocked()) {
                return false;
            }
        }
    }
    return true;
}

///
/// \brief Retrieves the current floor/level number.
/// \return An integer representing the level ID.
///
int GameModel::getCurrentLevel() const
{
    return currentLevel;
}

///
/// \brief Retrieves the total amount of gold collected by the player.
/// \return An integer representing the current gold balance.
///
int GameModel::getGoldAmount() const
{
    if (itsGoldModel != nullptr) {
        return itsGoldModel->getItsGold();
    }
    return 0;
}

///
/// \brief Deducts gold from the player's balance if they have enough.
/// \param amount The amount of gold to spend.
/// \return True if the transaction was successful, false if there are insufficient funds.
///
bool GameModel::spendGold(int amount) {
    if (getGoldAmount() >= amount) {
        itsGoldModel->removeGold(amount);
        return true;
    }
    return false;
}
