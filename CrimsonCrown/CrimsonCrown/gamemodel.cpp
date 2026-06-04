#include "gamemodel.h"
#include "blacksmithmodel.h"
#include "lancermodel.h"
#include "lordmodel.h"
#include "config.h"
#include "qpainterpath.h"
#include <QRandomGenerator> // Pour le spawn aléatoire

GameModel::GameModel() : itsScore(0), itsPlayer(nullptr)
{
    resetGame(HeroType::None);
    itsBoss = nullptr;
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

    // Ennemi de test
    itsEnemies.append(new LancerModel(50, 2, 1900, 300, 2.0f, 10));

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
const QList<EnemyModel*>& GameModel::getItsEnemies() const { return itsEnemies; }
PlayerModel *GameModel::getItsPlayer() const { return itsPlayer; }


// ==========================================
// FONCTION DE SPAWN ALÉATOIRE SÉCURISÉE
// ==========================================
void GameModel::spawnEnemiesInRoom(RoomModel* room, int count)
{
    if (!room) return;

    QRectF bounds = room->getBounds();

    int minX = bounds.left() + 50;
    int maxX = bounds.right() - 50;
    int minY = bounds.top() + 50;
    int maxY = bounds.bottom() - 50;

    for (int i = 0; i < count; ++i) {
        int randomX = QRandomGenerator::global()->bounded(minX, maxX);
        int randomY = QRandomGenerator::global()->bounded(minY, maxY);

        if (room->getType() == RoomType::Boss) {
            LordModel* boss = new LordModel(200, 1, randomX, randomY, 2.0f, 3.0f, 20, 30);
            itsBoss = boss; // CORRECTION : On range le Boss dans sa propre variable
            connect(boss, &LordModel::death, this, &GameModel::onBossDeath);
        } else {
            LancerModel* lancer = new LancerModel(50, 2, randomX, randomY, 2.0f, 10);
            itsEnemies.append(lancer); // Les Lanciers vont dans la liste d'ennemis
            connect(lancer, &LancerModel::death, this, [this, lancer]() { this->onEnemyDeath(lancer); });
        }
    }
}


void GameModel::updateLogic()
{
    // 1. UPDATE DE L'IA
    if (itsPlayer != nullptr) {
        // A. Mise à jour des Lanciers
        for (int i = itsEnemies.size() - 1; i >= 0; --i) {
            EnemyModel* enemy = itsEnemies.at(i);
            LancerModel* lancer = dynamic_cast<LancerModel*>(enemy);
            if (lancer) lancer->update(itsPlayer->getHitBox().center());
        }

        // B. Mise à jour du Boss (s'il existe sur la map)
        if (itsBoss != nullptr) {
            LordModel* lord = dynamic_cast<LordModel*>(itsBoss);
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

                    // Appelle notre fonction aléatoire avec le nombre d'ennemis défini !
                    spawnEnemiesInRoom(activeRoom, activeRoom->getRemainingEnemyCount());

                    // On vide la réserve et on ferme à clé
                    activeRoom->spawnEnemies(0);
                    activeRoom->lockDoors();
                }

                // 2) OUVERTURE DES PORTES (Si tous les ennemis sont morts)
                if (activeRoom->getIsLocked()) {
                    bool enemiesAliveInRoom = false;

                    // On vérifie les Lanciers
                    for (EnemyModel* enemy : itsEnemies) {
                        if (activeRoom->getBounds().intersects(enemy->getHitBox())) {
                            enemiesAliveInRoom = true;
                            break;
                        }
                    }

                    // On vérifie aussi le Boss
                    if (itsBoss != nullptr && activeRoom->getBounds().intersects(itsBoss->getHitBox())) {
                        enemiesAliveInRoom = true;
                    }

                    if (!enemiesAliveInRoom) {
                        activeRoom->unlockDoors();
                    }
                }
            }
        }

        // --- C. GESTION DES MURS ---
        bool collision = false;

        QPainterPath mapBounds;
        for (RoomModel* room : allRooms) {
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

        // --- D. COLLISIONS CAISSES ---
        for (RoomModel* room : allRooms) {
            for (ObstacleModel* obs : room->getItsObstacles()) {
                QRectF globalObsRect = obs->getHitBox();
                globalObsRect.translate(room->getBounds().x(), room->getBounds().y());

                if (itsPlayer->getHitBox().intersects(globalObsRect)) {
                    collision = true;
                }
            }
        }

        // E. APPLICATION DE LA PHYSIQUE
        if (collision) {
            itsPlayer->move(oldPos);
        }

        // F. SCORE : DÉCOUVERTE D'UNE NOUVELLE SALLE
        for (RoomModel* room : allRooms) {
            // Si la salle n'est pas explorée ET que le joueur marche dedans
            if (!room->getIsVisited() && itsPlayer->getHitBox().intersects(room->getBounds())) {
                room->setIsVisited(); // Découverte d'une salle

                // On ne donne pas 100 points pour un couloir, NI POUR LA SALLE DE SPAWN
                if (!room->getIsCorridor() && room->getType() != RoomType::Spawn) {
                    itsScore += 100;
                }
            }
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

    // Tue les lanciers
    for (int i = itsEnemies.size() - 1; i >= 0; --i) {
        if (activeRoom->getBounds().intersects(itsEnemies[i]->getHitBox())) {
            delete itsEnemies[i];
            itsEnemies.removeAt(i);
        }
    }

    // Tue le boss
    if (itsBoss != nullptr && activeRoom->getBounds().intersects(itsBoss->getHitBox())) {
        delete itsBoss;
        itsBoss = nullptr;
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
    if (qobjBoss) disconnect(qobjBoss);

    itsBoss = nullptr;
    itsScore += 150; // +150 Points pour le boss !

    delete model;
}
