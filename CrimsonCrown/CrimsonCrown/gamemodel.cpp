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
    combatRoom->addObstacle(1600, 200, 100, 100);
    combatRoom->addObstacle(2100, 400, 100, 100);
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
    // 1. UPDATE DU JOUEUR (Sans blocage sur les murs pour le moment)
    if (itsPlayer != nullptr) {
        itsPlayer->update();
    }

    // 2. UPDATE DE L'IA (Fluide)
    if (itsPlayer != nullptr) {
        for (EnemyModel* enemy : itsEnemies) {
            LancerModel* lancer = dynamic_cast<LancerModel*>(enemy);
            if (lancer) lancer->update(itsPlayer->getHitBox().center());

            LordModel* lord = dynamic_cast<LordModel*>(enemy);
            if (lord) lord->update(itsPlayer->getHitBox().center());
        }
    }
}