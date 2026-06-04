#include "gamemodel.h"
#include "blacksmithmodel.h"
#include "lancermodel.h"
#include "lordmodel.h"
#include "config.h"
#include <QPainterPath>

GameModel::GameModel() : itsScore(0), itsPlayer(nullptr)
{
    resetGame(HeroType::None);
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

    if (itsPlayer != nullptr) {
        delete itsPlayer;
        itsPlayer = nullptr;
    }

    // ==========================================
    // GÉNÉRATION DE LA CARTE (SANS LES ENNEMIS GLOBAUX)
    // ==========================================

    // 1. Spawn
    allRooms.append(new RoomModel(RoomType::Spawn, 0, 0, 600, 600));
    allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 600, 225, 400, 150));

    // 2. Combat 1
    RoomModel* combat1 = new RoomModel(RoomType::Combat, 1000, -25, 1100, 650);
    combat1->addObstacle(200, 150, 60, 60);
    combat1->addObstacle(800, 450, 60, 60);
    combat1->spawnEnemies(1); // <-- Indique à la salle qu'elle contient 1 ennemi en attente
    allRooms.append(combat1);

    allRooms.append(new RoomModel(RoomType::CorridorVertical, 1475, 625, 150, 400));

    // 3. Combat 2 (Carrefour)
    RoomModel* combat2 = new RoomModel(RoomType::Combat, 1000, 1025, 1100, 650);
    combat2->addObstacle(520, 300, 60, 60);
    combat2->spawnEnemies(1); // <-- Ennemi en attente
    allRooms.append(combat2);

    // 4. Loot
    allRooms.append(new RoomModel(RoomType::CorridorVertical, 1475, 1675, 150, 400));
    allRooms.append(new RoomModel(RoomType::Spawn, 1250, 2075, 600, 600));

    // 5. Combat 3
    allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 2100, 1275, 400, 150));
    RoomModel* combat3 = new RoomModel(RoomType::Combat, 2500, 1025, 1100, 650);
    combat3->spawnEnemies(1); // <-- Ennemi en attente
    allRooms.append(combat3);

    // 6. Boss
    allRooms.append(new RoomModel(RoomType::CorridorVertical, 2975, 625, 150, 400));
    RoomModel* bossRoom = new RoomModel(RoomType::Boss, 2500, -25, 1100, 650);
    bossRoom->spawnEnemies(1); // <-- Boss en attente
    allRooms.append(bossRoom);

    // 7. Combat 4
    allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 3600, 1275, 400, 150));
    RoomModel* combat4 = new RoomModel(RoomType::Combat, 4000, 1025, 1100, 650);
    combat4->spawnEnemies(1); // <-- Ennemi en attente
    allRooms.append(combat4);

    // Joueur
    switch(hero) {
    case HeroType::Blacksmith:
        itsPlayer = new BlacksmithModel();
        break;
    default:
        itsPlayer = nullptr;
        break;
    }

    if (itsPlayer != nullptr) {
        itsPlayer->move(QPointF(300, 300));
    }

    itsScore = 0;
}

const QList<RoomModel*>& GameModel::getAllRooms() const { return allRooms; }
void GameModel::spawnEnemiesForCurrentRoom(int count) {}

const QList<EnemyModel*>& GameModel::getItsEnemies() const { return itsEnemies; }


void GameModel::updateLogic()
{
    // 1. UPDATE DE L'IA
    if (itsPlayer != nullptr) {
        for (EnemyModel* enemy : itsEnemies) {
            LancerModel* lancer = dynamic_cast<LancerModel*>(enemy);
            if (lancer) lancer->update(itsPlayer->getHitBox().center());

            LordModel* lord = dynamic_cast<LordModel*>(enemy);
            if (lord) lord->update(itsPlayer->getHitBox().center());
        }
    }

    // 2. UPDATE DU JOUEUR & COLLISIONS
    if (itsPlayer != nullptr) {
        QPointF oldPos = itsPlayer->getHitBox().topLeft();
        itsPlayer->update();

        // --- A. DÉTECTION DE LA SALLE ACTIVE ---
        RoomModel* activeRoom = nullptr;
        for (RoomModel* room : allRooms) {
            if (room->getBounds().contains(itsPlayer->getHitBox().center())) {
                activeRoom = room;
                break;
            }
        }

        // --- B. SPAWN DES ENNEMIS ET PORTES ---
        if (activeRoom != nullptr) {
            RoomType type = activeRoom->getType();
            if (type == RoomType::Combat || type == RoomType::Boss) {

                // 1) DÉCLENCHEMENT DU PIÈGE (Le joueur entre pour la première fois)
                if (activeRoom->getRemainingEnemyCount() > 0 && activeRoom->getBounds().contains(itsPlayer->getHitBox())) {

                    // On calcule le centre exact de la salle
                    int spawnX = activeRoom->getOffsetX() + (activeRoom->getWidth() / 2) - (LancerWidth / 2);
                    int spawnY = activeRoom->getOffsetY() + (activeRoom->getHeight() / 2) - (LancerHeight / 2);

                    // ASTUCE : On met la vitesse à 0 pour qu'il soit totalement fixe !
                    itsEnemies.append(new LancerModel(50, 0, spawnX, spawnY, 2.0f, 100, 10));

                    // On indique que la salle est "vide" de sa réserve pour ne pas le spawner en boucle
                    activeRoom->spawnEnemies(0);

                    // On verrouille les portes
                    activeRoom->lockDoors();
                }

                // 2) OUVERTURE DES PORTES (Si tous les ennemis sont morts)
                if (activeRoom->getIsLocked()) {
                    bool enemiesAliveInRoom = false;
                    for (EnemyModel* enemy : itsEnemies) {
                        if (activeRoom->getBounds().intersects(enemy->getHitBox())) {
                            enemiesAliveInRoom = true;
                            break;
                        }
                    }

                    if (!enemiesAliveInRoom) {
                        activeRoom->unlockDoors();
                    }
                }
            }
        }

        // --- C. GESTION DES MURS ---
        bool collision = false;
        bool bossAccess = areAllCombatRoomsCleared(); // On vérifie si on a le droit de passer !

        if (activeRoom != nullptr && activeRoom->getIsLocked()) {
            if (!activeRoom->getBounds().contains(itsPlayer->getHitBox())) {
                collision = true;
            }
        }
        else {
            QPainterPath mapBounds;
            for (RoomModel* room : allRooms) {
                // LA MAGIE EST ICI : Si on n'a pas tout nettoyé, on retire le Boss de la carte !
                if (room->getType() == RoomType::Boss && !bossAccess) {
                    continue;
                }
                mapBounds.addRect(room->getOffsetX(), room->getOffsetY(),
                                  room->getWidth(), room->getHeight());
            }
            mapBounds = mapBounds.simplified();

            if (!mapBounds.contains(itsPlayer->getHitBox())) {
                collision = true;
            }
        }

        // --- D. COLLISIONS CAISSES ---
        for (RoomModel* room : allRooms) {
            for (ObstacleModel* obs : room->getItsObstacles()) {
                QRectF globalObsRect = obs->getHitBox();
                globalObsRect.translate(room->getOffsetX(), room->getOffsetY());

                if (itsPlayer->getHitBox().intersects(globalObsRect)) {
                    collision = true;
                }
            }
        }

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

    for (int i = itsEnemies.size() - 1; i >= 0; --i) {
        if (activeRoom->getBounds().intersects(itsEnemies[i]->getHitBox())) {
            delete itsEnemies[i];
            itsEnemies.removeAt(i);
        }
    }
}

PlayerModel *GameModel::getItsPlayer() const
{
    return itsPlayer;
}

bool GameModel::areAllCombatRoomsCleared() const
{
    for (RoomModel* room : allRooms) {
        if (room->getType() == RoomType::Combat) {
            // S'il reste des ennemis à faire spawner OU si la salle est verrouillée (combat en cours)
            if (room->getRemainingEnemyCount() > 0 || room->getIsLocked()) {
                return false; // On bloque l'accès !
            }
        }
    }
    return true; // Tout est nettoyé !
}