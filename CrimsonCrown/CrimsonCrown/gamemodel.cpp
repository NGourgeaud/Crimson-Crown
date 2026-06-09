#include "gamemodel.h"
#include "blacksmithmodel.h"
#include "assassinmodel.h"
#include "lancermodel.h"
#include "lordmodel.h"
#include "config.h"
#include "qdebug.h"
#include "qpainterpath.h"
#include <QRandomGenerator> // for random spawn
#include "huntermodel.h"

BossModel *GameModel::getItsBoss() const
{
    return itsBoss;
}

QList<ProjectileModel *> GameModel::getItsProjeciles() const
{
    return itsProjeciles;
}

GameModel::GameModel() : itsScore(0), itsPlayer(nullptr)
{
    currentLevel = 1;
    resetGame(HeroType::None);
    itsBoss = nullptr;
    activeRoom = nullptr;
}

GameModel::~GameModel()
{
    qDeleteAll(allRooms);
    allRooms.clear();
    if (itsPlayer != nullptr) {
        delete itsPlayer;
    }
}

void GameModel::resetGame(HeroType hero)
{
    qDeleteAll(allRooms);
    allRooms.clear();

    qDeleteAll(itsEnemies);
    itsEnemies.clear();

    if (itsBoss != nullptr) {
        delete itsBoss;
        itsBoss = nullptr;
    }

    if (itsPlayer != nullptr) {
        delete itsPlayer;
        itsPlayer = nullptr;
    }

    // ==========================================
    // MAP GENERATION (WITHOUT GLOBAL ENEMIES)
    // ==========================================

    // 1. Spawn
    allRooms.append(new RoomModel(RoomType::Spawn, 0, 0, 600, 600));
    allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 600, 225, 400, 150));

    // 2. Combat 1
    RoomModel* combat1 = new RoomModel(RoomType::Combat, 1000, -25, 1100, 650);
    combat1->addObstacle(200, 150, 60, 60);
    combat1->addObstacle(800, 450, 60, 60);
    allRooms.append(combat1);

    allRooms.append(new RoomModel(RoomType::CorridorVertical, 1475, 625, 150, 400));

    // 3. Combat 2 (Carrefour)
    RoomModel* combat2 = new RoomModel(RoomType::Combat, 1000, 1025, 1100, 650);
    combat2->addObstacle(520, 300, 60, 60);
    allRooms.append(combat2);

    // 4. Loot
    allRooms.append(new RoomModel(RoomType::CorridorVertical, 1475, 1675, 150, 400));
    allRooms.append(new RoomModel(RoomType::Spawn, 1250, 2075, 600, 600));

    // 5. Combat 3
    allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 2100, 1275, 400, 150));
    RoomModel* combat3 = new RoomModel(RoomType::Combat, 2500, 1025, 1100, 650);
    allRooms.append(combat3);

    // 6. Boss
    allRooms.append(new RoomModel(RoomType::CorridorVertical, 2975, 625, 150, 400));
    RoomModel* bossRoom = new RoomModel(RoomType::Boss, 2500, -25, 1100, 650);
    allRooms.append(bossRoom);

    // 7. Combat 4
    allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 3600, 1275, 400, 150));
    RoomModel* combat4 = new RoomModel(RoomType::Combat, 4000, 1025, 1100, 650);
    allRooms.append(combat4);

    // Player
    switch(hero) {
    case HeroType::Blacksmith: {
        BlacksmithModel * newPlayer = new BlacksmithModel();
        itsPlayer = newPlayer;
        connect(newPlayer, &BlacksmithModel::attackPlayer, this, [this](QPolygon hitbox, int damage) {this->onPlayerAttack(hitbox, damage);});
        break;
        }
    case HeroType::Assassin: {
        AssassinModel * newPlayer = new AssassinModel();
        itsPlayer = newPlayer;
        connect(newPlayer, &AssassinModel::attackPlayer, this, [this](QPolygon hitbox, int damage) {this->onPlayerAttack(hitbox, damage);});
        break;
    };
    case HeroType::Hunter: {
        HunterModel * newPlayer = new HunterModel();
        itsPlayer = newPlayer;
        connect(newPlayer, &HunterModel::attackPlayer, this, [this](QPolygon hitbox, int damage) {this->onPlayerAttack(hitbox, damage);});
        connect(newPlayer, &HunterModel::fireProjectile, this, &GameModel::onNewProjectile);
        break;
    }
    default:
        itsPlayer = nullptr;
        break;
    }

    if (itsPlayer != nullptr) {
        itsPlayer->move(QPointF(300, 300));
    }

    itsScore = 0;
    currentLevel = 1;
    loadLevel(currentLevel);
}

const QList<RoomModel*>& GameModel::getAllRooms() const { return allRooms; }
const QList<EnemyModel*>& GameModel::getItsEnemies() const { return itsEnemies; }
PlayerModel *GameModel::getItsPlayer() const { return itsPlayer; }


void GameModel::loadLevel(int levelId)
{
    // 1. Nettoyage complet de la carte précédente
    qDeleteAll(allRooms);
    allRooms.clear();
    qDeleteAll(itsEnemies);
    itsEnemies.clear();
    if (itsBoss) {
        delete itsBoss;
        itsBoss = nullptr;
    }
    activeRoom = nullptr;

    // 2. Génération de la carte selon le niveau demandé
    if (levelId == 1) {
        // --- MAQUETTE NIVEAU 1 EXACTE ---

        // 1. SPAWN (Tout à gauche)
        allRooms.append(new RoomModel(RoomType::Spawn, 0, 0, 600, 600));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 600, 225, 400, 150));

        // 2. COMBAT 1 (En haut à gauche)
        allRooms.append(new RoomModel(RoomType::Combat, 1000, -25, 1100, 650));

        // Couloir qui descend vers la salle violette
        allRooms.append(new RoomModel(RoomType::CorridorVertical, 1475, 625, 150, 400));

        // 3. COMBAT 2 (Le carrefour central, encadré en violet sur ta maquette)
        allRooms.append(new RoomModel(RoomType::Combat, 1000, 1025, 1100, 650));

        // 4. LOOT (Branche du bas)
        allRooms.append(new RoomModel(RoomType::CorridorVertical, 1475, 1675, 150, 400));
        allRooms.append(new RoomModel(RoomType::Spawn, 1250, 2075, 600, 600)); // Type Spawn utilisé pour Loot

        // 5. COMBAT 3 (Branche de droite)
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 2100, 1275, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, 2500, 1025, 1100, 650));

        // 6. COMBAT 4 (Branche tout à droite)
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 3600, 1275, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, 4000, 1025, 1100, 650));

        // 7. BOSS (Branche du haut depuis Combat 3)
        // Le couloir monte : position Y inversée (-400)
        allRooms.append(new RoomModel(RoomType::CorridorVertical, 2975, 625, 150, 400));
        allRooms.append(new RoomModel(RoomType::Boss, 2500, -25, 1100, 650));

        // 8. SORTIE (Au-dessus du Boss)
        allRooms.append(new RoomModel(RoomType::Exit, 2975, -425, 150, 400));
    }
    else if (levelId == 2) {
        // --- NIVEAU 2 (Fidèle à ta nouvelle maquette) ---
        allRooms.append(new RoomModel(RoomType::Spawn, 0, 0, 600, 600)); // Spawn à droite
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -400, 225, 400, 150));

        allRooms.append(new RoomModel(RoomType::Combat, -1500, -25, 1100, 650)); // Centre principal

        // Branche du HAUT
        allRooms.append(new RoomModel(RoomType::CorridorVertical, -1025, -425, 150, 400));
        allRooms.append(new RoomModel(RoomType::Combat, -1500, -1075, 1100, 650));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -400, -825, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, 0, -1075, 1100, 650));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -1900, -825, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, -3000, -1075, 1100, 650));

        // Branche GAUCHE (Boss de fin de jeu)
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -1900, 225, 400, 150));
        allRooms.append(new RoomModel(RoomType::Boss, -3000, -25, 1100, 650));

        // LA PIÈCE "SORTIE" DE TA MAQUETTE SOUS LE BOSS DU LVL 2
        allRooms.append(new RoomModel(RoomType::Exit, -2525, 625, 150, 400));

        // Branche du BAS
        allRooms.append(new RoomModel(RoomType::CorridorVertical, -1025, 625, 150, 400));
        allRooms.append(new RoomModel(RoomType::Combat, -1500, 1025, 1100, 650));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -400, 1275, 400, 150));
        allRooms.append(new RoomModel(RoomType::Spawn, 0, 1075, 600, 600)); // Zone Loot droite

        allRooms.append(new RoomModel(RoomType::CorridorVertical, -1025, 1675, 150, 400));
        allRooms.append(new RoomModel(RoomType::Combat, -1500, 2075, 1100, 650));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -1900, 2325, 400, 150));
        allRooms.append(new RoomModel(RoomType::Spawn, -2500, 2100, 600, 600)); // Zone Loot gauche
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -2900, 2325, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, -4000, 2075, 1100, 650));
    }

    // 3. Repositionner le personnage au centre de la zone de Spawn actuelle
    if (itsPlayer != nullptr) {
        for (RoomModel* room : allRooms) {
            if (room->getType() == RoomType::Spawn) {
                itsPlayer->move(room->getBounds().center());
                break;
            }
        }
    }
}


void GameModel::spawnEnemies(bool isBoss)
{
    QRect roomBoundaries = activeRoom->getBounds().toRect();
    if (!isBoss){
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> EnemiesNb(4,6);
        std::uniform_int_distribution<int> RoomX(roomBoundaries.topLeft().rx(),roomBoundaries.topRight().rx());
        std::uniform_int_distribution<int> RoomY(roomBoundaries.topLeft().ry(),roomBoundaries.bottomLeft().ry());

        int nbEnemies = EnemiesNb(eng);
        for (int i = 0; i < nbEnemies; i++){
            LancerModel * newEnnemy = new LancerModel(LancerDefaultMaxHp, LancerDefaultSpeed, RoomX(eng), RoomY(eng), LancerDefaultAttackCoolDown, LancerDefaultAttackDamage);
            itsEnemies.append(newEnnemy);

            // Using a lambda to secure parameter passing
            connect(newEnnemy, &LancerModel::death, this, [this, newEnnemy]() {this->onEnemyDeath(newEnnemy);});
            connect(newEnnemy, &LancerModel::attacking, this, [this](QPolygon hitbox, int damage) {this->onEnemyAttack(hitbox, damage);});
        }
    }
    else{
        LordModel * newLord = new LordModel(LordDefaultMaxHp, LordDefaultSpeed, roomBoundaries.center().rx(), roomBoundaries.center().ry(),
                                           LordDefaultAttack1CoolDown, LordDefaultAttack2CoolDown, LordDefaultAttack1Damage, LordDefaultAttack2Damage);
        itsBoss = newLord;

        connect(newLord, &LordModel::death, this, &GameModel::onBossDeath);
        connect(newLord, &LordModel::attacking, this, [this](QPolygon hitbox, int damage) {this->onEnemyAttack(hitbox, damage);});
    }

}

void GameModel::updateLogic()
{
    // 1. AI UPDATE
    if (itsPlayer != nullptr) {
        // A. Lancers Update
        for (int i = itsEnemies.size() - 1; i >= 0; --i) {
            EnemyModel* enemy = itsEnemies.at(i);
            LancerModel* lancer = dynamic_cast<LancerModel*>(enemy);
            if (lancer) lancer->update(itsPlayer->getHitBox().center());
        }

        // B. Boss update (if it exists on the map)
        if (itsBoss != nullptr) {
            LordModel* lord = dynamic_cast<LordModel*>(itsBoss);
            if (lord) lord->update(itsPlayer->getHitBox().center());
        }
    }

    // projectile update
    if (!itsProjeciles.empty()){
        for (ProjectileModel * pro : itsProjeciles){
            pro->update();
        }
    }

    // 2. PLAYER UPDATE & COLLISIONS
    if (itsPlayer != nullptr) {
        QPointF oldPos = itsPlayer->getHitBox().topLeft();
        itsPlayer->update();

        // --- A. DETECTION OF THE ACTIVITY ROOM ---
        for (RoomModel* room : allRooms) {
            if (room->getBounds().contains(itsPlayer->getHitBox().center())) {
                activeRoom = room;
                break;
            }
        }

        // --- B. DISCOVERY OF A NEW ROOM AND SPAWN OF ENEMIES ---
        if (activeRoom != nullptr) {
            RoomType type = activeRoom->getType();

            // =========================================================
            // GESTION DU COULOIR DE SORTIE (TP Niveau 2 OU Victoire)
            // =========================================================
            if (type == RoomType::Exit) {
                if (currentLevel == 1) {
                    // On passe au niveau 2
                    currentLevel = 2;
                    loadLevel(currentLevel); // Charge la nouvelle carte et TP le joueur au spawn du Lvl 2
                    return; // On quitte la fonction pour ce tick, la nouvelle map est chargée !
                }
                else if (currentLevel == 2) {
                    // Fin du niveau 2 : Le joueur prend la sortie, il a GAGNÉ le jeu !
                    emit gameWon(itsScore); // Déclenche ton écran de victoire final
                    return;
                }
            }

            // --- Logique classique de combat si la pièce n'a pas encore été visitée ---
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

        // --- C. WALL MANAGEMENT ---
        bool collision = false;
        bool bossAccess = areAllCombatRoomsCleared();

        QPainterPath mapBounds;
        for (RoomModel* room : allRooms) {
            if (room->getType() == RoomType::Boss && !bossAccess) {
                continue;
            }
            mapBounds.addRect(room->getOffsetX(), room->getOffsetY(),
                              room->getWidth(), room->getHeight());
        }
        mapBounds = mapBounds.simplified();

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

        // --- D. COLLISIONS CARRIERS ---
        for (RoomModel* room : allRooms) {
            for (ObstacleModel* obs : room->getItsObstacles()) {
                QRectF globalObsRect = obs->getHitBox();
                globalObsRect.translate(room->getBounds().x(), room->getBounds().y());

                if (itsPlayer->getHitBox().intersects(globalObsRect)) {
                    collision = true;
                }
            }
        }

        // E. APPLICATION OF PHYSICS
        if (collision) {
            itsPlayer->move(oldPos);
        }
    }
}

void GameModel::cheatKillEnemies()
{
    if (!itsPlayer) return;
    RoomModel* activeRoom = nullptr;
    for (RoomModel* room : allRooms) {
        if (room->getBounds().contains(itsPlayer->getHitBox().center())) {
            activeRoom = room;
            break;
        }
    }
    if (!activeRoom) return;

    // Kill the lancers
    for (int i = itsEnemies.size() - 1; i >= 0; --i) {
        if (activeRoom->getBounds().intersects(itsEnemies[i]->getHitBox())) {
            itsEnemies[i]->takeDamage(999999);
        }
    }

    // Kill the boss
    if (itsBoss != nullptr && activeRoom->getBounds().intersects(itsBoss->getHitBox())) {
        itsBoss->takeDamage(9999999);
    }
}

int GameModel::getItsScore() const
{
    return itsScore;
}


void GameModel::onEnemyDeath(EnemyModel * model)
{
    itsEnemies.removeOne(model);
    itsScore += 10; // +10 Points pour le lancier

    if (itsEnemies.empty()){
        activeRoom->unlockDoors();
        itsPlayer->regen();
    }

    QObject * qobjEnemy = dynamic_cast<QObject*>(model);
    if (qobjEnemy) {
        qobjEnemy->deleteLater();
    }
    else {
        delete model;
    }

}

void GameModel::onBossDeath(BossModel * model)
{
    QObject * qobjBoss = dynamic_cast<QObject*>(model);
    if (activeRoom != nullptr && activeRoom->getType() == RoomType::Boss) {
        activeRoom->unlockDoors();
    }
    if (qobjBoss) disconnect(qobjBoss);

    itsBoss = nullptr;
    itsScore += 150;

    // V2
    // currentRoom->unlockDoors();


    //emit gameWon(itsScore);

    delete model;
}

void GameModel::onEnemyAttack(QPolygon hitbox, int damage)
{
    emit attack(hitbox);
    if (hitbox.intersects(itsPlayer->getHitBox().toRect())){
        itsPlayer->takeDamage(damage);
    }
}

void GameModel::onPlayerAttack(QPolygon hitbox, int damage)
{
    emit attack(hitbox);
    if (!itsEnemies.empty()){
        for (EnemyModel* enemy : itsEnemies){
            if (hitbox.intersects(enemy->getHitBox().toRect())){
                enemy->takeDamage(damage);
            }
        }
    }
    else if(itsBoss){
        if (hitbox.intersects(itsBoss->getHitBox().toRect())){
            itsBoss->takeDamage(damage);
        }
    }
}

void GameModel::onNewProjectile(ProjectileModel * projectile)
{
    itsProjeciles.append(projectile);
    if (projectile->getItsTeam() == ENEMY)
        connect(projectile, &ProjectileModel::tick, this, &GameModel::onEnemyProjectile);
    else
        connect(projectile, &ProjectileModel::tick, this, &GameModel::onplayerProjectile);
}

void GameModel::onEnemyProjectile(ProjectileModel * projectile)
{
    if (projectile->getItsHitbox().intersects(itsPlayer->getHitBox())){
        itsPlayer->takeDamage(projectile->getItsDamage());
        disconnect(projectile);
        itsProjeciles.removeOne(projectile);
        projectile->deleteLater();
    }

    if (!activeRoom->getItsObstacles().empty()){
        for (ObstacleModel * obstacle : activeRoom->getItsObstacles()){
            // Création d'une Hitbox globale comme pour le joueur
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

void GameModel::onplayerProjectile(ProjectileModel *projectile)
{
    if(!itsEnemies.empty()){
        for (EnemyModel * enemy : itsEnemies){
            if (projectile->getItsHitbox().intersects(enemy->getHitBox())){
                enemy->takeDamage(projectile->getItsDamage());
                disconnect(projectile);
                itsProjeciles.removeOne(projectile);
                delete projectile;
                return;
            }
        }
    }
    else if (itsBoss){
        if (projectile->getItsHitbox().intersects(itsBoss->getHitBox())){
            itsBoss->takeDamage(projectile->getItsDamage());
            disconnect(projectile);
            itsProjeciles.removeOne(projectile);
            delete projectile;
            return;
        }
    }


    if (!activeRoom->getItsObstacles().empty()){
        for (ObstacleModel * obstacle : activeRoom->getItsObstacles()){
            // Création d'une Hitbox globale
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
    //    and is floating in the middle. Otherwise, it's stuck in the border zone.
    if (!innerSpace.contains(projectile->getItsHitbox())){
        disconnect(projectile);
       itsProjeciles.removeOne(projectile);
       delete projectile;
       return;
    }
}

bool GameModel::areAllCombatRoomsCleared() const
{
    for (RoomModel* room : allRooms) {
        if (room->getType() == RoomType::Combat) {
            //If a combat room has not yet been visited OR is still locked (combat in progress)
            if (!room->getIsVisited() || room->getIsLocked()) {
                return false;
            }
        }
    }
    return true;
}
