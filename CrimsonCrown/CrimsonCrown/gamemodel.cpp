#include "gamemodel.h"
#include "blacksmithmodel.h"
#include "lancermodel.h"
#include "lordmodel.h"

GameModel::GameModel()
    : itsScore(0), itsPlayer(nullptr)
{
    // On initialise le jeu au démarrage avec aucun héros choisi par défaut
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
    // 1. Nettoyage si une partie était déjà en cours
    qDeleteAll(allRooms);
    allRooms.clear();

    qDeleteAll(itsEnemies);
    itsEnemies.clear();

    if (itsPlayer != nullptr) {
        delete itsPlayer;
        itsPlayer = nullptr;
    }

    // ==========================================
    // 2. GÉNÉRATION DE LA CARTE COMPLÈTE
    // ==========================================

    // Salle 1 : Spawn (1000x600)
    allRooms.append(new RoomModel(RoomType::Spawn, 0, 0, 1000, 600));

    // Couloir Horizontal : Plus large (250 de haut) et centré
    allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 1000, 175, 400, 250));

    // Salle 2 : Combat (1000x600)
    RoomModel* combatRoom = new RoomModel(RoomType::Combat, 1400, 0, 1000, 600);
    // Les caisses ont des coordonnées LOCALES (à l'intérieur de la salle)
    combatRoom->addObstacle(200, 100, 100, 100);
    combatRoom->addObstacle(700, 400, 100, 100);
    allRooms.append(combatRoom);

    // TEMPORAIRE : Ennemi de test
    itsEnemies.append(new LancerModel(50, 3, 1900, 300, 2.0f, 100, 10));

    // Couloir Vertical : Plus large (250 de large) et centré
    allRooms.append(new RoomModel(RoomType::CorridorVertical, 1775, -400, 250, 400));

    // Salle 3 : Boss (1000x600)
    allRooms.append(new RoomModel(RoomType::Boss, 1400, -1000, 1000, 600));

    // --- Placement du joueur ---
    if (itsPlayer != nullptr) {
        itsPlayer->move(QPointF(500, 250)); // Bien aligné face au couloir !
    }

    // 3. Création du Joueur
    switch(hero) {
    case HeroType::Blacksmith:
        itsPlayer = new BlacksmithModel();
        break;
    case HeroType::None:
    default:
        itsPlayer = nullptr;
        break;
    }

    if (itsPlayer != nullptr) {
        // On place le joueur au centre de la salle de Spawn
        itsPlayer->move(QPointF(400, 225));
    }

    itsScore = 0;
}

const QList<RoomModel*>& GameModel::getAllRooms() const
{
    return allRooms;
}

void GameModel::spawnEnemiesForCurrentRoom(int count)
{
    // Sera adapté plus tard pour l'IA
}

#include <QPainterPath> // A AJOUTER TOUT EN HAUT DU FICHIER !

void GameModel::updateLogic()
{
    // 1. UPDATE DE L'IA (Mouvements fluides des ennemis)
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
        itsPlayer->update(); // Le joueur avance

        bool collision = false;

        // --- MAGIE DES MURS : On fusionne tout le donjon ! ---
        QPainterPath mapBounds;
        for (RoomModel* room : allRooms) {
            // On ajoute 2 pixels de marge de chaque côté pour effacer les "coutures"
            mapBounds.addRect(room->getOffsetX() - 2, room->getOffsetY() - 2,
                              room->getWidth() + 4, room->getHeight() + 4);
        }
        mapBounds = mapBounds.simplified();

        // Si le joueur n'est plus entièrement dans le donjon, il a tapé un mur extérieur !
        if (!mapBounds.contains(itsPlayer->getHitBox())) {
            collision = true;
        }

        // --- COLLISION AVEC LES CAISSES ---
        for (RoomModel* room : allRooms) {
            for (ObstacleModel* obs : room->getItsObstacles()) {
                // Les caisses sont locales à la salle, on calcule leur vraie position sur la carte
                QRectF globalObsRect = obs->getHitBox();
                globalObsRect.translate(room->getOffsetX(), room->getOffsetY());

                if (itsPlayer->getHitBox().intersects(globalObsRect)) {
                    collision = true;
                }
            }
        }

        // --- APPLICATION PHYSIQUE ---
        // Si on a touché quelque chose, on annule le mouvement
        if (collision) {
            itsPlayer->move(oldPos);
        }
    }
}

PlayerModel *GameModel::getItsPlayer() const
{
    return itsPlayer;
}