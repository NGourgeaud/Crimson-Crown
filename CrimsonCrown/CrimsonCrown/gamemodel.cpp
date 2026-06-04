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
    // 1. UPDATE DE L'IA DES ENNEMIS
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
        // A. On sauvegarde l'ancienne position avant de bouger
        QPointF oldPos = itsPlayer->getHitBox().topLeft();

        // B. Le joueur fait son mouvement
        itsPlayer->update();

        bool collision = false;

        // C. VÉRIFICATION DES MURS EXTÉRIEURS
        QPainterPath mapBounds;
        for (RoomModel* room : allRooms) {
            // On dessine le plan exact de chaque pièce et on les fusionne (simplified)
            mapBounds.addRect(room->getOffsetX(), room->getOffsetY(),
                              room->getWidth(), room->getHeight());
        }
        mapBounds = mapBounds.simplified();

        // Si la HitBox du joueur n'est plus à 100% à l'intérieur du plan du donjon, il touche un mur !
        if (!mapBounds.contains(itsPlayer->getHitBox())) {
            collision = true;
        }

        // D. VÉRIFICATION DES CAISSES
        for (RoomModel* room : allRooms) {
            for (ObstacleModel* obs : room->getItsObstacles()) {
                // On transforme la coordonnée Locale de la caisse en coordonnée Globale
                QRectF globalObsRect = obs->getHitBox();
                globalObsRect.translate(room->getOffsetX(), room->getOffsetY());

                // Si le joueur rentre dans la caisse
                if (itsPlayer->getHitBox().intersects(globalObsRect)) {
                    collision = true;
                }
            }
        }

        // E. APPLICATION DE LA PHYSIQUE
        // Si on a touché un mur ou une caisse, on annule le mouvement en remettant l'ancienne position !
        if (collision) {
            itsPlayer->move(oldPos);
        }
    }
}
