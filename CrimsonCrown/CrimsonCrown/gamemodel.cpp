#include "gamemodel.h"
#include "archermodel.h"
#include "blacksmithmodel.h"
#include "assassinmodel.h"
#include "knightmodel.h"
#include "lancermodel.h"
#include "lordmodel.h"
#include "config.h"
#include "qpainterpath.h"
#include <QRandomGenerator> // for random spawn
#include <QtCore/qdebug.h>
#include "huntermodel.h"

BossModel *GameModel::getItsBoss() const
{
    return itsBoss;
}

QList<ProjectileModel *> GameModel::getItsProjeciles() const
{
    return itsProjeciles;
}

GameModel::GameModel() : itsScore(0), itsPlayer(nullptr)
{
    currentLevel = 1;
    itsBoss = nullptr;
    activeRoom = nullptr;
    minEnemy = 4;
    maxEnemy = 6;
    statIncreass = 1;
    isEasterEggFound = false;
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

    isHeart1Collected = false;
    isHeart2Collected = false;

    qDeleteAll(allRooms);
    allRooms.clear();

    qDeleteAll(itsEnemies);
    itsEnemies.clear();

    qDeleteAll(itsProjeciles);
    itsProjeciles.clear();

    if (itsBoss != nullptr) {
        delete itsBoss;
        itsBoss = nullptr;
    }

    if (itsPlayer != nullptr) {
        delete itsPlayer;
        itsPlayer = nullptr;
    }

    // ==========================================
    // MAP GENERATION (WITHOUT GLOBAL ENEMIES)
    // ==========================================

    // 1. Spawn
    allRooms.append(new RoomModel(RoomType::Spawn, 0, 0, 600, 600));
    allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 600, 225, 400, 150));

    // 2. Combat 1
    RoomModel* combat1 = new RoomModel(RoomType::Combat, 1000, -25, 1100, 650);
    combat1->addObstacle(200, 150, 60, 60);
    combat1->addObstacle(800, 450, 60, 60);
    allRooms.append(combat1);

    allRooms.append(new RoomModel(RoomType::CorridorVertical, 1475, 625, 150, 400));

    // 3. Combat 2 (Carrefour)
    RoomModel* combat2 = new RoomModel(RoomType::Combat, 1000, 1025, 1100, 650);
    combat2->addObstacle(520, 300, 60, 60);
    allRooms.append(combat2);

    // 4. Loot
    allRooms.append(new RoomModel(RoomType::CorridorVertical, 1475, 1675, 150, 400));
    allRooms.append(new RoomModel(RoomType::Spawn, 1250, 2075, 600, 600));

    // 5. Combat 3
    allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 2100, 1275, 400, 150));
    RoomModel* combat3 = new RoomModel(RoomType::Combat, 2500, 1025, 1100, 650);
    allRooms.append(combat3);

    // 6. Boss
    allRooms.append(new RoomModel(RoomType::CorridorVertical, 2975, 625, 150, 400));
    RoomModel* bossRoom = new RoomModel(RoomType::Boss, 2500, -25, 1100, 650);
    allRooms.append(bossRoom);

    // 7. Combat 4
    allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 3600, 1275, 400, 150));
    RoomModel* combat4 = new RoomModel(RoomType::Combat, 4000, 1025, 1100, 650);
    allRooms.append(combat4);

    // Player
    switch(hero) {
    case HeroType::Blacksmith: {
        BlacksmithModel * newPlayer = new BlacksmithModel();
        itsPlayer = newPlayer;
        connect(newPlayer, &BlacksmithModel::attackPlayer, this, [this](HitZoneModel hitbox, int damage) {this->onPlayerAttack(hitbox, damage);});
        break;
        }
    case HeroType::Assassin: {
        AssassinModel * newPlayer = new AssassinModel();
        itsPlayer = newPlayer;
        connect(newPlayer, &AssassinModel::attackPlayer, this, [this](HitZoneModel hitbox, int damage) {this->onPlayerAttack(hitbox, damage);});
        connect(newPlayer, &AssassinModel::fireProjectile, this, &GameModel::onNewProjectile);
        break;
    };
    case HeroType::Hunter: {
        HunterModel * newPlayer = new HunterModel();
        itsPlayer = newPlayer;
        connect(newPlayer, &HunterModel::attackPlayer, this, [this](HitZoneModel hitbox, int damage) {this->onPlayerAttack(hitbox, damage);});
        connect(newPlayer, &HunterModel::fireProjectile, this, &GameModel::onNewProjectile);
        break;
    }
    default:
        itsPlayer = nullptr;
        break;
    }

    if (itsPlayer != nullptr) {
        itsPlayer->move(QPointF(300, 300));
    }

    itsScore = 0;
    currentLevel = 1;
    loadLevel(currentLevel);
}

const QList<RoomModel*>& GameModel::getAllRooms() const { return allRooms; }
const QList<EnemyModel*>& GameModel::getItsEnemies() const { return itsEnemies; }
PlayerModel *GameModel::getItsPlayer() const { return itsPlayer; }


void GameModel::loadLevel(int levelId)
{
    // 1. Nettoyage complet de la carte précédente
    qDeleteAll(allRooms);
    allRooms.clear();
    qDeleteAll(itsEnemies);
    itsEnemies.clear();
    if (itsBoss) {
        delete itsBoss;
        itsBoss = nullptr;
    }
    activeRoom = nullptr;

    // 2. Génération de la carte selon le niveau demandé
    if (levelId == 1) {
        // --- MAQUETTE NIVEAU 1 EXACTE ---

        // 1. SPAWN (Tout à gauche)
        allRooms.append(new RoomModel(RoomType::Spawn, 0, 0, 600, 600));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 600, 225, 400, 150));

        // 2. COMBAT 1 (En haut à gauche)
        allRooms.append(new RoomModel(RoomType::Combat, 1000, -25, 1100, 650));
        allRooms.last()->addObstacle(250, 250, 100, 100);


        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 2100, 225, 400, 150));

        // Couloir qui descend vers la salle violette
        allRooms.append(new RoomModel(RoomType::CorridorVertical, 1475, 625, 150, 400));

        // 3. COMBAT 2 (Le carrefour central, encadré en violet sur ta maquette)
        allRooms.append(new RoomModel(RoomType::Combat, 1000, 1025, 1100, 650));

        // 4. LOOT (Branche du bas)
        allRooms.append(new RoomModel(RoomType::CorridorVertical, 1475, 1675, 150, 400));
        allRooms.append(new RoomModel(RoomType::Spawn, 1250, 2075, 600, 600)); // Type Spawn utilisé pour Loot

        // 5. COMBAT 3 (Branche de droite)
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 2100, 1275, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, 2500, 1025, 1100, 650));

        // 6. COMBAT 4 (Branche tout à droite)
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, 3600, 1275, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, 4000, 1025, 1100, 650));

        // 7. BOSS (Branche du haut depuis Combat 3)
        // Le couloir monte : position Y inversée (-400)
        allRooms.append(new RoomModel(RoomType::CorridorVertical, 2975, 625, 150, 400));
        allRooms.append(new RoomModel(RoomType::Boss, 2500, -25, 1100, 650));

        // 8. SORTIE (Au-dessus du Boss)
        allRooms.append(new RoomModel(RoomType::Exit, 2975, -425, 150, 400));
    }
    else if (levelId == 2) {
        // --- NIVEAU 2 (Fidèle à ta nouvelle maquette) ---
        allRooms.append(new RoomModel(RoomType::Spawn, 0, 0, 600, 600)); // Spawn à droite
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -400, 225, 400, 150));

        allRooms.append(new RoomModel(RoomType::Combat, -1500, -25, 1100, 650)); // Centre principal

        // Branche du HAUT
        allRooms.append(new RoomModel(RoomType::CorridorVertical, -1025, -425, 150, 400));
        allRooms.append(new RoomModel(RoomType::Combat, -1500, -1075, 1100, 650));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -400, -825, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, 0, -1075, 1100, 650));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -1900, -825, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, -3000, -1075, 1100, 650));

        // Branche GAUCHE (Boss de fin de jeu)
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -1900, 225, 400, 150));
        allRooms.append(new RoomModel(RoomType::Boss, -3000, -25, 1100, 650));

        // LA PIÈCE "SORTIE" DE TA MAQUETTE SOUS LE BOSS DU LVL 2
        allRooms.append(new RoomModel(RoomType::Exit, -2525, 625, 150, 400));

        // Branche du BAS
        allRooms.append(new RoomModel(RoomType::CorridorVertical, -1025, 625, 150, 400));
        allRooms.append(new RoomModel(RoomType::Combat, -1500, 1025, 1100, 650));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -400, 1275, 400, 150));
        allRooms.append(new RoomModel(RoomType::Spawn, 0, 1075, 600, 600)); // Zone Loot droite

        allRooms.append(new RoomModel(RoomType::CorridorVertical, -1025, 1675, 150, 400));
        allRooms.append(new RoomModel(RoomType::Combat, -1500, 2075, 1100, 650));
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -1900, 2325, 400, 150));
        allRooms.append(new RoomModel(RoomType::Spawn, -2500, 2100, 600, 600)); // Zone Loot gauche
        allRooms.append(new RoomModel(RoomType::CorridorHorizontal, -2900, 2325, 400, 150));
        allRooms.append(new RoomModel(RoomType::Combat, -4000, 2075, 1100, 650));
    }

    // 3. Repositionner le personnage au centre de la zone de Spawn actuelle
    if (itsPlayer != nullptr) {
        for (RoomModel* room : allRooms) {
            if (room->getType() == RoomType::Spawn) {
                itsPlayer->move(room->getBounds().center());
                break;
            }
        }
    }

}


void GameModel::spawnEnemies(bool isBoss)
{
    QRect roomBoundaries = activeRoom->getBounds().toRect();
    if (!isBoss){
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> RoomX(roomBoundaries.topLeft().rx()+60,roomBoundaries.topRight().rx()-60);
        std::uniform_int_distribution<int> RoomY(roomBoundaries.topLeft().ry()+60,roomBoundaries.bottomLeft().ry()-60);
        std::uniform_int_distribution<int> Type(LANCER,ARCHER);

        if (currentLevel == 2){
            minEnemy = 5;
            maxEnemy = 7;
            statIncreass+=0.1;
            qDebug() << "Level 2";
        }
        std::uniform_int_distribution<int> EnemiesNb(minEnemy,maxEnemy);

        int nbEnemies = EnemiesNb(eng);
        for (int i = 0; i < nbEnemies; i++){
            switch (Type(eng)) {
            case LANCER:
            {
                LancerModel * newEnnemy = new LancerModel(LancerDefaultMaxHp*statIncreass, LancerDefaultSpeed, RoomX(eng), RoomY(eng), LancerDefaultAttackCoolDown, LancerDefaultAttackDamage*statIncreass);
                itsEnemies.append(newEnnemy);

                // Using a lambda to secure parameter passing
                connect(newEnnemy, &LancerModel::death, this, [this, newEnnemy]() {this->onEnemyDeath(newEnnemy);});
                connect(newEnnemy, &LancerModel::attacking, this, [this](HitZoneModel hitbox, int damage) {this->onEnemyAttack(hitbox, damage);});
                break;
            }
            case KNIGHT:
            {
                KnightModel * newEnnemy = new KnightModel(KnightDefaultMaxHp, KnightDefaultSpeed, RoomX(eng), RoomY(eng), KnightDefaultAttackCoolDown, KnightDefaultAttackDamage);
                itsEnemies.append(newEnnemy);

                // Using a lambda to secure parameter passing
                connect(newEnnemy, &KnightModel::death, this, [this, newEnnemy]() {this->onEnemyDeath(newEnnemy);});
                connect(newEnnemy, &KnightModel::attacking, this, [this](HitZoneModel hitbox, int damage) {this->onEnemyAttack(hitbox, damage);});
                break;
            }
            case ARCHER:
            {
                ArcherModel * newEnnemy = new ArcherModel(ArcherDefaultMaxHp, ArcherDefaultSpeed, RoomX(eng), RoomY(eng), ArcherDefaultAttackCoolDown, ArcherDefaultAttackDamage);
                itsEnemies.append(newEnnemy);

                // Using a lambda to secure parameter passing
                connect(newEnnemy, &ArcherModel::death, this, [this, newEnnemy]() {this->onEnemyDeath(newEnnemy);});
                connect(newEnnemy, &ArcherModel::attacking, this, &GameModel::onNewProjectile);
                break;
            }
            default:
                break;
            }
        }
    }
    else{
        LordModel * newLord = new LordModel(LordDefaultMaxHp, LordDefaultSpeed, roomBoundaries.center().rx(), roomBoundaries.center().ry(),
                                           LordDefaultAttack1CoolDown, LordDefaultAttack2CoolDown, LordDefaultAttack1Damage, LordDefaultAttack2Damage);
        itsBoss = newLord;

        connect(newLord, &LordModel::death, this, &GameModel::onBossDeath);
        connect(newLord, &LordModel::attacking, this, [this](HitZoneModel hitbox, int damage) {this->onEnemyAttack(hitbox, damage);});
    }

}

void GameModel::updateLogic()
{
    // 1. AI UPDATE
    if (itsPlayer != nullptr) {
        bool isPlayerInvisible = false;
        AssassinModel* assassin = dynamic_cast<AssassinModel*>(itsPlayer);
        if (assassin && assassin->getIsInvisible()) {
            isPlayerInvisible = true;
        }

        // --- C'EST ICI QU'ON REMET LA VÉRIFICATION D'INVISIBILITÉ ---
        if (!isPlayerInvisible) {

            // A. Enemies Update
            for (int i = itsEnemies.size() - 1; i >= 0; --i) {

                bool isCollision = false;

                EnemyModel* enemy = itsEnemies.at(i);

                // 1. MISE À JOUR DE L'IA (L'ennemi calcule sa trajectoire et avance vers le joueur en premier)
                enemy->update(itsPlayer->getHitBox().center());

                // 2. Collisions avec les autres ennemis
                for (int j = 0; j < itsEnemies.size(); ++j) {
                    EnemyModel * otherEnemy = itsEnemies.at(j);

                    // On ignore la collision avec soi-même
                    if (enemy == otherEnemy) {
                        continue;
                    }

                    QRectF teamCollision = enemy->getHitBox().intersected(otherEnemy->getHitBox());

                    // Si les deux ennemis se rentrent dedans
                    if (!teamCollision.isEmpty()) {

                        // On récupère la position actuelle pour la corriger
                        QPointF newPos = enemy->getHitBox().topLeft();
                        QRectF otherRect = otherEnemy->getHitBox();

                        // CAS 1 : Collision en haut ou en bas (Poussée verticale)
                        if (teamCollision.width() > teamCollision.height()) {
                            if(enemy->getHitBox().center().y() < otherRect.center().y()){
                                newPos.setY(otherRect.top() - enemy->getHitBox().height());
                            }
                            else{
                                newPos.setY(otherRect.bottom());
                            }
                        }

                        // CAS 2 : Collision sur les côtés (Poussée horizontale)
                        else if (teamCollision.height() > teamCollision.width()) {
                            if(enemy->getHitBox().center().x() < otherRect.center().x()){
                                newPos.setX(otherRect.left() - enemy->getHitBox().width());
                            }
                            else{
                                newPos.setX(otherRect.right());
                            }
                        }

                        // CAS 3 : Collision pile sur un coin
                        else {
                            if(enemy->getHitBox().center().y() < otherRect.center().y()){
                                newPos.setY(otherRect.top() - enemy->getHitBox().height());
                            }
                            else{
                                newPos.setY(otherRect.bottom());
                            }

                            if(enemy->getHitBox().center().x() < otherRect.center().x()){
                                newPos.setX(otherRect.left() - enemy->getHitBox().width());
                            }
                            else{
                                newPos.setX(otherRect.right());
                            }
                        }

                        // On applique la répulsion pour qu'ils ne se superposent pas
                        enemy->move(newPos);

                        isCollision = true;
                    }
                }

                // 3. Collision avec les bords de la map
                QPointF newPosMap = enemy->getHitBox().topLeft();
                QRectF roomBounds = activeRoom->getBounds();

                if (newPosMap.x() < roomBounds.left()) {
                    newPosMap.setX(roomBounds.left());
                }
                else if (newPosMap.x() + enemy->getHitBox().width() > roomBounds.right()) {
                    newPosMap.setX(roomBounds.right() - enemy->getHitBox().width());
                }

                if (newPosMap.y() < roomBounds.top()) {
                    newPosMap.setY(roomBounds.top());
                }
                else if (newPosMap.y() + enemy->getHitBox().height() > roomBounds.bottom()) {
                    newPosMap.setY(roomBounds.bottom() - enemy->getHitBox().height());
                }
                enemy->move(newPosMap);

                // 4. Collision avec les obstacles
                if (!activeRoom->getItsObstacles().empty()){

                    QRectF zoneCollision;
                    // On récupère la position absolue que l'ennemi vient de calculer
                    QPointF newPos = enemy->getHitBox().topLeft();

                    for (ObstacleModel * obstacle : activeRoom->getItsObstacles()) {

                        // Calcul de la HitBox Globale de l'obstacle
                        QRectF globalObsRect = obstacle->getHitBox();
                        globalObsRect.translate(activeRoom->getBounds().x(), activeRoom->getBounds().y());

                        zoneCollision = enemy->getHitBox().intersected(globalObsRect);

                        // S'il y a une collision avec cet obstacle
                        if(!zoneCollision.isEmpty()){

                            // CAS 1 : Collision en haut ou en bas (Mur Horizontal)
                            if (zoneCollision.width() > zoneCollision.height()) {

                                // Répulsion contre le mur
                                if(enemy->getHitBox().center().y() < globalObsRect.center().y()){
                                    newPos.setY(globalObsRect.top() - enemy->getHitBox().height());
                                }
                                else{
                                    newPos.setY(globalObsRect.bottom());
                                }

                                // --- SMART SLIDING (SANS RESTRICTION + FRICTION) ---
                                // L'ennemi glisse TOUJOURS vers la position X du joueur
                                if (itsPlayer->getHitBox().center().x() > enemy->getHitBox().center().x()) {
                                    newPos.setX(newPos.x() + (enemy->getitsSpeed())); // Glisse à droite
                                } else {
                                    newPos.setX(newPos.x() - (enemy->getitsSpeed())); // Glisse à gauche
                                }
                            }

                            // CAS 2 : Collision sur les côtés (Mur Vertical)
                            else if (zoneCollision.height() > zoneCollision.width()) {

                                // Répulsion contre le mur
                                if(enemy->getHitBox().center().x() < globalObsRect.center().x()){
                                    newPos.setX(globalObsRect.left() - enemy->getHitBox().width());
                                }
                                else{
                                    newPos.setX(globalObsRect.right());
                                }

                                // --- SMART SLIDING (SANS RESTRICTION + FRICTION) ---
                                // L'ennemi glisse TOUJOURS vers la position Y du joueur
                                if (itsPlayer->getHitBox().center().y() > enemy->getHitBox().center().y()) {
                                    newPos.setY(newPos.y() + (enemy->getitsSpeed())); // Glisse vers le bas
                                } else {
                                    newPos.setY(newPos.y() - (enemy->getitsSpeed())); // Glisse vers le haut
                                }
                            }

                            // CAS 3 : Collision pile sur un coin (largeur == hauteur)
                            else {
                                // Répulsion simple en diagonale
                                if(enemy->getHitBox().center().y() < globalObsRect.center().y()){
                                    newPos.setY(globalObsRect.top() - enemy->getHitBox().height());
                                }
                                else{
                                    newPos.setY(globalObsRect.bottom());
                                }

                                if(enemy->getHitBox().center().x() < globalObsRect.center().x()){
                                    newPos.setX(globalObsRect.left() - enemy->getHitBox().width());
                                }
                                else{
                                    newPos.setX(globalObsRect.right());
                                }
                            }

                            // APPLICATION DU DEPLACEMENT
                            enemy->move(newPos);

                            isCollision = true;
                        }
                    }
                }
                if(!isCollision){
                    enemy->update(itsPlayer->getHitBox().center());
                }
            }
        }

    }

    if (itsBoss){
        itsBoss->update(itsPlayer->getHitBox().center());
    }

    // projectile update
    if (!itsProjeciles.empty()){
        for (ProjectileModel * pro : itsProjeciles){
            pro->update();
        }
    }

    // 2. PLAYER UPDATE & COLLISIONS
    if (itsPlayer != nullptr) {
        QPointF oldPos = itsPlayer->getHitBox().topLeft();
        itsPlayer->update();

        // --- A. DETECTION OF THE ACTIVITY ROOM ---
        for (RoomModel* room : allRooms) {
            if (room->getBounds().contains(itsPlayer->getHitBox().center())) {
                activeRoom = room;
                break;
            }
        }

        // --- B. DISCOVERY OF A NEW ROOM AND SPAWN OF ENEMIES ---
        if (activeRoom != nullptr) {
            RoomType type = activeRoom->getType();

            // =========================================================
            // GESTION DU COULOIR DE SORTIE (TP Niveau 2 OU Victoire)
            // =========================================================
            if (type == RoomType::Exit) {
                if (currentLevel == 1) {
                    // On passe au niveau 2
                    currentLevel = 2;
                    loadLevel(currentLevel); // Charge la nouvelle carte et TP le joueur au spawn du Lvl 2
                    return; // On quitte la fonction pour ce tick, la nouvelle map est chargée !
                }
                else if (currentLevel == 2) {
                    // Fin du niveau 2 : Le joueur prend la sortie, il a GAGNÉ le jeu !
                    emit gameWon(itsScore, itsPlayer->getItsName()); // Déclenche ton écran de victoire final
                    return;
                }
            }

            // --- Logique classique de combat si la pièce n'a pas encore été visitée ---
            if (!activeRoom->getIsVisited() && activeRoom->getBounds().contains(itsPlayer->getHitBox())){
                activeRoom->setIsVisited();

                if (type == RoomType::Combat) {
                    spawnEnemies(false);
                    itsScore += 100;
                    activeRoom->lockDoors();
                }
                else if (type == RoomType::Boss){
                    spawnEnemies(true);
                    itsScore += 100;
                    activeRoom->lockDoors();
                }
            }
        }
        // ==========================================
        // --- LOGIQUE DES COEURS (NIVEAU 2) ---
        // ==========================================
        if (currentLevel == 2) {

            // Coeur 1 (Dans la salle de loot de droite, centre = 300, 1375)
            if (!isHeart1Collected) {
                QRectF heart1Zone(275, 1350, 50, 50); // Hitbox de 50x50 au centre
                if (itsPlayer->getHitBox().intersects(heart1Zone)) {
                    itsPlayer->heal(5); // +5 HP !
                    isHeart1Collected = true;
                    qDebug() << "Coeur 1 ramasse !";
                }
            }

            // Coeur 2 (Dans la salle de loot tout a gauche, centre = -2200, 2400)
            if (!isHeart2Collected) {
                QRectF heart2Zone(-2225, 2375, 50, 50); // Hitbox de 50x50 au centre
                if (itsPlayer->getHitBox().intersects(heart2Zone)) {
                    itsPlayer->heal(5); // +5 HP !
                    isHeart2Collected = true;
                    qDebug() << "Coeur 2 ramasse !";
                }
            }
        }

        // ==========================================
        // --- LOGIQUE EASTER EGG & COULOIR SECRET ---
        // ==========================================

        // 1. Détecter si le boss est mort
        bool isBossDead = false;
        for (RoomModel* room : allRooms) {
            if (room->getType() == RoomType::Boss && room->getIsVisited() && itsBoss == nullptr) {
                isBossDead = true;
                break;
            }
        }

        if (currentLevel == 1) {

            // --- SÉCURITÉ : ON NE FAIT ÇA QUE SI LE COFFRE N'EST PAS ENCORE OUVERT ---
            if (!isEasterEggFound) {

                // A. Ramasser la clef
                QRectF secretKeyZone(2430, 275, 50, 50);
                if (isBossDead && !itsPlayer->getHasSecretKey() && itsPlayer->getHitBox().intersects(secretKeyZone)) {
                    itsPlayer->setHasSecretKey(true);
                    qDebug() << "Clef secrete trouvee dans le couloir invisible !";
                }

                // B. Ouvrir le coffre
                QRectF chestZone(1500, 2325, 100, 100);
                QRectF interactionZone = chestZone.adjusted(-20, -20, 20, 20);

                if (itsPlayer->getHasSecretKey() && itsPlayer->getHitBox().intersects(interactionZone)) {
                    qDebug() << "COFFRE OUVERT ! EASTER EGG DECOUVERT !";
                    itsPlayer->setHasSecretKey(false);
                    itsScore += 200;

                    // ON VERROUILLE L'EASTER EGG POUR LE RESTE DE LA PARTIE
                    isEasterEggFound = true;
                }
            }
        }

        // --- C. WALL MANAGEMENT (MIS À JOUR POUR LE COULOIR INVISIBLE) ---
        bool collision = false;
        bool bossAccess = areAllCombatRoomsCleared();

        QPainterPath mapBounds;
        for (RoomModel* room : allRooms) {
            if (room->getType() == RoomType::Boss && !bossAccess) {
                continue;
            }

            // --- MODIFICATION COLLISION SECRÈTE ---
            // Si c'est notre couloir secret et que le boss n'est pas mort, on ne l'ajoute pas.
            if (room->getOffsetX() == 2100 && room->getOffsetY() == 225 && !isBossDead) {
                continue;
            }

            mapBounds.addRect(room->getOffsetX(), room->getOffsetY(),
                              room->getWidth(), room->getHeight());
        }
        mapBounds = mapBounds.simplified();

        // 1. Détection des bords de la carte et des portes fermées
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

        // --- D. COLLISIONS OBSTACLES (Tonneaux, etc.) ---
        for (RoomModel* room : allRooms) {
            for (ObstacleModel* obs : room->getItsObstacles()) {
                QRectF globalObsRect = obs->getHitBox();
                globalObsRect.translate(room->getBounds().x(), room->getBounds().y());

                if (itsPlayer->getHitBox().intersects(globalObsRect)) {
                    collision = true;
                }
            }
        }

        // --- E. APPLICATION DE LA PHYSIQUE ---
        if (collision) {
            itsPlayer->move(oldPos);
        }
    } // <-- Fin du grand "if (itsPlayer != nullptr)"
} // <-- Fin de la fonction updateLogic()

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

    // Kill the lancers
    for (int i = itsEnemies.size() - 1; i >= 0; --i) {
        if (activeRoom->getBounds().intersects(itsEnemies[i]->getHitBox())) {
            itsEnemies[i]->takeDamage(999999);
        }
    }

    // Kill the boss
    if (itsBoss != nullptr && activeRoom->getBounds().intersects(itsBoss->getHitBox())) {
        itsBoss->takeDamage(9999999);
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

    if (itsEnemies.empty()){
        activeRoom->unlockDoors();
        itsPlayer->regen();
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
    if (activeRoom != nullptr && activeRoom->getType() == RoomType::Boss) {
        activeRoom->unlockDoors();
    }
    if (qobjBoss) disconnect(qobjBoss);

    itsBoss = nullptr;
    itsScore += 150;

    // V2
    // currentRoom->unlockDoors();


    //emit gameWon(itsScore);

    delete model;
}

void GameModel::onEnemyAttack(HitZoneModel hitbox, int damage)
{
    emit attack(hitbox);
    if (hitbox.getItsPolygon().intersects(itsPlayer->getHitBox().toRect())){
        itsPlayer->takeDamage(damage);
    }
}

void GameModel::onPlayerAttack(HitZoneModel hitbox, int damage)
{
    emit attack(hitbox);
    if (!itsEnemies.empty()){
        for (EnemyModel* enemy : itsEnemies){
            if (hitbox.getItsPolygon().intersects(enemy->getHitBox().toRect())){
                enemy->takeDamage(damage);
            }
        }
    }
    else if(itsBoss){
        if (hitbox.getItsPolygon().intersects(itsBoss->getHitBox().toRect())){
            itsBoss->takeDamage(damage);
        }
    }
}

void GameModel::onNewProjectile(ProjectileModel * projectile)
{
    itsProjeciles.append(projectile);
    if (projectile->getItsTeam() == ENEMY)
        connect(projectile, &ProjectileModel::tick, this, &GameModel::onEnemyProjectile);
    else
        connect(projectile, &ProjectileModel::tick, this, &GameModel::onplayerProjectile);
}

void GameModel::onEnemyProjectile(ProjectileModel * projectile)
{
    if (projectile->getItsHitbox().intersects(itsPlayer->getHitBox())){
        itsPlayer->takeDamage(projectile->getItsDamage());
        disconnect(projectile);
        itsProjeciles.removeOne(projectile);
        projectile->deleteLater();
    }

    if (!activeRoom->getItsObstacles().empty()){
        for (ObstacleModel * obstacle : activeRoom->getItsObstacles()){
            // Création d'une Hitbox globale comme pour le joueur
            QRectF globalObsRect = obstacle->getHitBox();
            globalObsRect.translate(activeRoom->getBounds().x(), activeRoom->getBounds().y());

            if (projectile->getItsHitbox().intersects(globalObsRect)){
                disconnect(projectile);
                itsProjeciles.removeOne(projectile);
                delete projectile;
                return;
            }
        }
    }

    // Define the inner area (the hollow space past the border)
    QMarginsF margins(1, 1, 1, 1);
    QRectF innerSpace = activeRoom->getBounds().marginsRemoved(margins);

    // If innerSpace fully contains r1, it means r1 safely bypassed the border
    // and is floating in the middle. Otherwise, it's stuck in the border zone.
    if (!innerSpace.contains(projectile->getItsHitbox())){
        disconnect(projectile);
        itsProjeciles.removeOne(projectile);
        delete projectile;
        return;
    }
}

void GameModel::onplayerProjectile(ProjectileModel *projectile)
{
    if(!itsEnemies.empty()){
        for (EnemyModel * enemy : itsEnemies){
            if (projectile->getItsHitbox().intersects(enemy->getHitBox())){
                enemy->takeDamage(projectile->getItsDamage());
                disconnect(projectile);
                itsProjeciles.removeOne(projectile);
                delete projectile;
                return;
            }
        }
    }
    else if (itsBoss){
        if (projectile->getItsHitbox().intersects(itsBoss->getHitBox())){
            itsBoss->takeDamage(projectile->getItsDamage());
            disconnect(projectile);
            itsProjeciles.removeOne(projectile);
            delete projectile;
            return;
        }
    }


    if (!activeRoom->getItsObstacles().empty()){
        for (ObstacleModel * obstacle : activeRoom->getItsObstacles()){
            // Création d'une Hitbox globale
            QRectF globalObsRect = obstacle->getHitBox();
            globalObsRect.translate(activeRoom->getBounds().x(), activeRoom->getBounds().y());

            if (projectile->getItsHitbox().intersects(globalObsRect)){
                disconnect(projectile);
                itsProjeciles.removeOne(projectile);
                delete projectile;
                return;
            }
        }
    }

    // Define the inner area (the hollow space past the border)
    QMarginsF margins(1, 1, 1, 1);
    QRectF innerSpace = activeRoom->getBounds().marginsRemoved(margins);

    // If innerSpace fully contains r1, it means r1 safely bypassed the border
    //    and is floating in the middle. Otherwise, it's stuck in the border zone.
    if (!innerSpace.contains(projectile->getItsHitbox())){
        disconnect(projectile);
       itsProjeciles.removeOne(projectile);
       delete projectile;
       return;
    }
}

bool GameModel::areAllCombatRoomsCleared() const
{
    for (RoomModel* room : allRooms) {
        if (room->getType() == RoomType::Combat) {
            //If a combat room has not yet been visited OR is still locked (combat in progress)
            if (!room->getIsVisited() || room->getIsLocked()) {
                return false;
            }
        }
    }
    return true;
}

int GameModel::getCurrentLevel() const
{
    return currentLevel;
}
