#include "gamemodel.h"

GameModel::GameModel()
    : currentRoom(nullptr) /*, itsPlayer(nullptr)*/, itsScore(0)
{
    // On initialise le jeu au démarrage
    resetGame();
}

GameModel::~GameModel()
{
    delete currentRoom;
    // delete itsPlayer;
}

void GameModel::resetGame()
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

void GameModel::updateLogic()
{
    if (!currentRoom) return;

    // 1. OPENING OF THE PREVIOUS ROOM
    // Si la salle est verrouillée mais qu'il n'y a plus d'ennemis
    if (currentRoom->getIsLocked() && currentRoom->getRemainingEnemyCount() <= 0) {

        currentRoom->unlockDoors(); // On ouvre les portes
    }

    // ==========================================
    // 2. TO RUN THE GAUNTLET (Déclenchement de la transition)
    // ==========================================
    // Ici, il faut vérifier si le joueur (quand il sera implémenté) touche la porte de sortie.
    // Exemple : ( ia )
    /*
    if (!currentRoom->getIsLocked()) {
        QRect zoneSortie(450, 600, 100, 20); // La zone de la porte
        if (itsPlayer->getHitBox().intersects(zoneSortie)) {
            goToNextRoom();
        }
    }
    */
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