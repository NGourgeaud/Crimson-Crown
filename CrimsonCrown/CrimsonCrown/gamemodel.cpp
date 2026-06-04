#include "gamemodel.h"
#include "blacksmithmodel.h"
#include "lancermodel.h"
#include "lordmodel.h"

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

    // GÉNÉRATION DE LA CARTE

    // salle spawn : petit carré
    allRooms.append(new RoomModel(RoomType::Spawn, 0, 0, 600, 600));

    // couloir horizontal centré sur le mur droit
    allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 600, 225, 400, 150));

    // Salle 2 : Combat (1000x600)
    RoomModel* combatRoom = new RoomModel(RoomType::Combat, 1000, 0, 1125, 675);
    combatRoom->addObstacle(200, 150, 60, 60);
    combatRoom->addObstacle(800, 450, 60, 60);
    allRooms.append(combatRoom);

    // Ennemi de test
    itsEnemies.append(new LancerModel(50, 3, 1900, 300, 2.0f, 100, 10));

    // Couloir Vertical
    allRooms.append(new RoomModel(RoomType::CorridorVertical, 1475, -400, 150, 400));

    // Salle 3 : Boss
    allRooms.append(new RoomModel(RoomType::Boss, 1050, -1000, 1000, 600));

    // Création du Joueur
    switch(hero) {
    case HeroType::Blacksmith:
        itsPlayer = new BlacksmithModel();
        break;
    default:
        itsPlayer = nullptr;
        break;
    }

    if (itsPlayer != nullptr) {
        itsPlayer->move(QPointF(500, 300)); // Au centre exact du Spawn !
    }

    itsScore = 0;
}

const QList<RoomModel*>& GameModel::getAllRooms() const { return allRooms; }
void GameModel::spawnEnemiesForCurrentRoom(int count) {}
PlayerModel *GameModel::getItsPlayer() const { return itsPlayer; }


void GameModel::updateLogic()
{
    // 1. UPDATE DE L'IA DES ENNEMIS ET GESTION DU SCORE
    if (itsPlayer != nullptr) {
        for (int i = itsEnemies.size() - 1; i >= 0; --i) {
            EnemyModel* enemy = itsEnemies.at(i);

            // SCORE : Si l'ennemi est mort
            if (enemy->getItsHP() <= 0) {
                // Si c'est un Boss (LordModel), +150 pts ! Sinon +10 pts.
                if (dynamic_cast<LordModel*>(enemy)) {
                    itsScore += 150;
                } else {
                    itsScore += 10;
                }

                itsEnemies.removeAt(i);
                delete enemy;
                continue;
            }

            // IA s'il est vivant
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

        bool collision = false;

        // C. VÉRIFICATION DES MURS EXTÉRIEURS (Avec getBounds)
        QPainterPath mapBounds;
        for (RoomModel* room : allRooms) {
            mapBounds.addRect(room->getBounds());
        }
        mapBounds = mapBounds.simplified();

        if (!mapBounds.contains(itsPlayer->getHitBox())) {
            collision = true;
        }

        // D. VÉRIFICATION DES CAISSES (Avec getBounds)
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

int GameModel::getItsScore() const
{
    return itsScore;
}

