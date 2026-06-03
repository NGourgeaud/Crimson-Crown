#include "gamemodel.h"
#include "blacksmithmodel.h"
#include "lancermodel.h"

GameModel::GameModel()
    : currentRoom(nullptr), itsScore(0), itsPlayer(nullptr)
{
    // On initialise le jeu au démarrage
    resetGame();
}

GameModel::~GameModel()
{
    delete currentRoom;
    delete itsPlayer;
}

void GameModel::resetGame()
{
    // 1. Nettoyage si une partie était déjà en cours
    if (currentRoom != nullptr) {
        delete currentRoom;
    }

    if (itsPlayer != nullptr) {
        delete itsPlayer;
    }

    // "First hall"
    // On crée la première salle. Par defaut SpawnHall,
    // elle n'est pas verrouillée et n'a pas d'ennemis
    currentRoom = new RoomModel(false);

    // On peut ajouter quelques obstacles purement décoratifs
    // pour que la salle de spawn ne soit pas complètement vide.
    currentRoom->addObstacle(200, 200, 50, 50);
    currentRoom->addObstacle(600, 200, 50, 50);


    //"Player spawn"
    // On instancie le joueur
    itsPlayer = new BlacksmithModel();
    // On place le joueur au centre de la salle.
    itsPlayer->move(400, 225); // au centre a redefini

    itsScore = 0;

    roomCount = 1;
    nextisCorridor = true; // apres la salle -> veut un couloir
}

const RoomModel* GameModel::getCurrentRoom() const
{
    return currentRoom;
}


void GameModel::spawnEnemiesForCurrentRoom(int count)
{
    // On indique à la salle combien d'ennemis elle contient
    currentRoom->spawnEnemies(count);

    // On nettoie les anciens ennemis
    qDeleteAll(itsEnemies);
    itsEnemies.clear();

    // On crée les nouveaux ennemis
    for(int i = 0; i < count; i++) {
        // intancier les nouveaus enemies
    }
}


void GameModel::updateLogic()
{
    // a implémenter
}

void GameModel::goToNextRoom()
{
    // 1. On nettoie l'ancienne salle et les anciens ennemis
    if (currentRoom != nullptr) {
        delete currentRoom;
    }
    qDeleteAll(itsEnemies);
    itsEnemies.clear();

    // 2. On choisit ce qu'on génère
    if (nextisCorridor) {
        // GÉNÉRATION D'UN COULOIR
        currentRoom = new RoomModel(true, false); // true pour isCorridor
        nextisCorridor = false; // La prochaine zone sera une salle
    }
    else {
        // CAS B : GÉNÉRATION D'UNE SALLE (Normale ou Boss)
        roomCount++; // On incrémente le nombre de salles visitées
        nextisCorridor = true; // La prochaine zone sera un couloir

        // Est-ce que c'est la N ieme salle ? (Le Boss)
        if (roomCount == 5) { // 5 pour l'exemple
            currentRoom = new RoomModel(false, true); // true pour isBossRoom
            currentRoom->lockDoors();

            // on met un LancerModel
            itsEnemies.append(new LancerModel(150, 1, 400, 300, 2.0f, 100, 10));
            currentRoom->spawnEnemies(1); // Le Boss compte pour 1 ennemi

            // Si le joueur bat le boss, le niveau d'après recommencera à zéro
            roomCount = 0;
        }
        else {
            // C'est une salle normale
            currentRoom = new RoomModel(false, false);
            currentRoom->lockDoors();
            currentRoom->addObstacle(300, 200, 50, 50);

            // On utilise ta méthode pour faire spawner les ennemis de base
            spawnEnemiesForCurrentRoom(5);
        }
    }
}

PlayerModel *GameModel::getItsPlayer() const
{
    return itsPlayer;
}
