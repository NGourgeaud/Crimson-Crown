#include "gamemodel.h"
#include "blacksmithmodel.h"

GameModel::GameModel()
    : currentRoom(nullptr) ,itsPlayer(nullptr), itsScore(0)
{
    // On initialise le jeu au démarrage
    resetGame(HeroType::None);
}

GameModel::~GameModel()
{
    delete currentRoom;
    delete itsPlayer;
}

void GameModel::resetGame(HeroType hero)
{
    // 1. Nettoyage si une partie était déjà en cours
    if (currentRoom != nullptr) {
        delete currentRoom;
    }
    /*
    if (itsPlayer != nullptr) {
        delete itsPlayer;
    }
    */

    // "First hall"
    // On crée la première salle. Par defaut SpawnHall,
    // elle n'est pas verrouillée et n'a pas d'ennemis.
    currentRoom = new RoomModel();

    // On peut ajouter quelques obstacles purement décoratifs
    // pour que la salle de spawn ne soit pas complètement vide.
    currentRoom->addObstacle(200, 200, 50, 50);
    currentRoom->addObstacle(600, 200, 50, 50);

    switch(hero) {
    case HeroType::Blacksmith:
        itsPlayer = new BlacksmithModel();
        break;
    case HeroType::None:
    default:
        itsPlayer = nullptr;
        break;
    }

    //"Player spawn"
    // On instancie le joueur
    // itsPlayer = new BlackSmithModel();
    // On place le joueur au centre de la salle.
    // itsPlayer->setPosition(400, 225); // au centre

    itsScore = 0;
}

const RoomModel* GameModel::getCurrentRoom() const
{
    return currentRoom;
}

/*
void GameModel::spawnEnemiesForCurrentRoom(int count)
{
    // On indique à la salle combien d'ennemis elle contient
    currentRoom->spawnEnemies(count);

    // On nettoie les anciens ennemis s'il y en avait
    qDeleteAll(itsEnemies);
    itsEnemies.clear();

    // On crée les nouveaux ennemis
    for(int i = 0; i < count; i++) {
        // C'est ici que tu instancieras tes vrais ennemis plus tard
        // Par exemple : itsEnemies.append(new LancerModel(...));
    }
}
*/

void GameModel::updateLogic()
{
    // a implémenter
}

// Méthode dédiée pour générer et passer à la salle suivante
void GameModel::goToNextRoom()
{
    // On nettoie l'ancienne salle
    if (currentRoom != nullptr) {
        delete currentRoom;
    }

    // On instancie la nouvelle salle
    currentRoom = new RoomModel();

    // On peut générer de nouveaux obstacles
    currentRoom->addObstacle(300, 200, 50, 50);


    // 2. CLOSE THE NEXT ROOM

    currentRoom->lockDoors(); // On enferme le joueur

    // On fait apparaître la nouvelle vague d'ennemis
    //valeur fixe (ex: 5) pour tester.
    currentRoom->spawnEnemies(5);

    // On replace le joueur à l'entrée de cette nouvelle salle
    // itsPlayer->setPosition(400, 100); // Coordonnées de l'entrée en haut par exemple a adapter en fonction de la salle
}

PlayerModel *GameModel::getItsPlayer() const
{
    return itsPlayer;
}
