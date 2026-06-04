#include "gamemodel.h"
#include "blacksmithmodel.h"
#include "lancermodel.h"
#include "lordmodel.h"
#include <random>

GameModel::GameModel()
    : currentRoom(nullptr), itsScore(0), itsPlayer(nullptr)
{
    // On initialise le jeu au démarrage avec aucun héros choisi par défaut
    resetGame(HeroType::None); //
}

GameModel::~GameModel()
{
    delete currentRoom; //
    delete itsPlayer; //
}

void GameModel::resetGame(HeroType hero)
{
    // 1. Nettoyage si une partie était déjà en cours
    if (currentRoom != nullptr) { //
        delete currentRoom; //
    }

    if (itsPlayer != nullptr) { //
        delete itsPlayer; //
        itsPlayer = nullptr;
    }

    // "First hall"
    // On crée la première salle. Par défaut SpawnHall,
    // elle n'est pas verrouillée et n'a pas d'ennemis
    currentRoom = new RoomModel(false); //

    // On peut ajouter quelques obstacles purement décoratifs
    // pour que la salle de spawn ne soit pas complètement vide.
    currentRoom->addObstacle(200, 200, 50, 50); //
    currentRoom->addObstacle(600, 200, 50, 50); //

    switch(hero) {
    case HeroType::Blacksmith:
        itsPlayer = new BlacksmithModel();
        break;
    case HeroType::None:
    default:
        itsPlayer = nullptr; //
        break;
    }


    if (itsPlayer != nullptr) {
        // On place le joueur au centre de la salle.
        itsPlayer->move(QPointF(400, 225));
    }

    itsScore = 0; //

    roomCount = 1; //
    nextisCorridor = true; // apres la salle -> veut un couloir
}

const RoomModel* GameModel::getCurrentRoom() const
{
    return currentRoom;
}

void GameModel::spawnEnemies(bool isBoss)
{
    QRect roomBoundaries = currentRoom->getItsWalls();
    if (!isBoss){
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> EnemiesNb(4,6);
        std::uniform_int_distribution<int> RoomX(roomBoundaries.topLeft().rx(),roomBoundaries.topRight().rx());
        std::uniform_int_distribution<int> RoomY(roomBoundaries.topLeft().ry(),roomBoundaries.bottomLeft().ry());

        int nbEnemies = EnemiesNb(eng);
        for (int i = 0; i < nbEnemies; i++){
            LancerModel * newEnnemy = new LancerModel(LancerDefaultMaxHp, LancerDefaultSpeed, RoomX(eng), RoomY(eng), LancerDefaultAttackCoolDown, LancerDefaultAttackDamage);
            itsEnemies.append(newEnnemy);

            // Utilisation d'une lambda pour sécuriser le passage de paramètre
            connect(newEnnemy, &LancerModel::death, this, [this, newEnnemy]() {this->onEnemyDeath(newEnnemy);});
        }
    }
    else{
        LordModel * newLord = new LordModel(LordDefaultMaxHp, LordDefaultSpeed, roomBoundaries.center().rx(), roomBoundaries.center().ry(),
                                            LordDefaultAttack1CoolDown, LordDefaultAttack2CoolDown, LordDefaultAttack1Damage, LordDefaultAttack2Damage);
        itsBoss = newLord;

        connect(newLord, &LordModel::death, this, &GameModel::onBossDeath);
    }
}

void GameModel::updateLogic()
{
    // a implémenter
}

void GameModel::goToNextRoom()
{
    // 1. On nettoie l'ancienne salle et les anciens ennemis
    if (currentRoom != nullptr) { //
        delete currentRoom; //
    }
    qDeleteAll(itsEnemies); //
    itsEnemies.clear(); //

    // 2. On choisit ce qu'on génère
    if (nextisCorridor) { //
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
            currentRoom->lockDoors(); //

            // CORRECTION CONSTRUCTEUR DU BOSS : Correspond à la signature de lancermodel.h
            // LancerModel(int maxHp, int speed, int x, int y, float attackCooldown, int maxAttackDistance, int attackDamage)
            spawnEnemies(true); // Le Boss compte pour 1 ennemi

            // Si le joueur bat le boss, le niveau d'après recommencera à zéro
            roomCount = 0; //
        }
        else {
            // C'est une salle normale
            currentRoom = new RoomModel(false, false); //
            currentRoom->lockDoors(); //
            currentRoom->addObstacle(300, 200, 50, 50); //

            // On utilise ta méthode pour faire spawner les ennemis de base
            spawnEnemies(false); //
        }
    }
}

PlayerModel *GameModel::getItsPlayer() const
{
    return itsPlayer; //
}

void GameModel::onEnemyDeath(EnemyModel * model)
{
    itsEnemies.removeOne(model);

    if (itsEnemies.empty()){
        currentRoom->unlockDoors();
    }

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
    disconnect(qobjBoss);
    itsBoss = nullptr;
    delete model;

    itsScore += 150;
    // V2
    // currentRoom->unlockDoors();
}
