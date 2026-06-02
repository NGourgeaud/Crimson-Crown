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