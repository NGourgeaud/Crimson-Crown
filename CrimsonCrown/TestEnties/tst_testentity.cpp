#include <QTest>
#include <QSignalSpy>

// add necessary includes here
#include "../CrimsonCrown/archermodel.h"
#include "../CrimsonCrown/lancermodel.h"
#include "../CrimsonCrown/knightmodel.h"
#include "QtWidgets/qpushbutton.h"
#include "charselectview.h"
#include "gamemodel.h"
#include "leaderboardmodel.h"
#include "menuview.h"

class TestEntity : public QObject
{
    Q_OBJECT

public:
    TestEntity();
    ~TestEntity();

private slots:
    void init();
    void cleanup();

    // Tests pour l'Archer
    void test_archer_move();
    void test_archer_takeDamage_and_death();
    void test_archer_handleAI();

    // Tests pour le Lancer
    void test_lancer_move();
    void test_lancer_takeDamage_and_death();
    void test_lancer_handleAI();

    // Tests pour le Knight
    void test_knight_move();
    void test_knight_takeDamage_and_death();
    void test_knight_handleAI();

    // Tests pour le leaderbord
    void initTestCase();
    void cleanupTestCase();
    void testEmptyBoard();
    void testAddSingleScore();
    void testSortingScores();
    void testMaxCapacity();
    void testEmptyNameAndZeroScore();

    // Tests pour le menu
    void testMenuInitialization();
    void testMenuPlaySignalWithoutSpy();
    void testCharSelectInitialization();
    void testCarouselWrapAroundRight();
    void testCarouselWrapAroundLeft();

    // Tests pour les collisions
    void test_collision_mur_limite();
    void test_collision_entre_ennemis();
    void test_smart_sliding_obstacle();

private:
    // Variables par défaut gérées par init() et cleanup()
    ArcherModel* archer;
    LancerModel* lancer;
    KnightModel* knight;

    LeaderboardModel* board;
    QString realFilePath = "scores.txt";
    QString backupFilePath = "scores_test.txt";

    MenuView* menu;
    CharSelectView* charSelect;

    GameModel *game;
    LancerModel *testEnemy1;
    LancerModel *testEnemy2;
};

TestEntity::TestEntity() {}

TestEntity::~TestEntity() {}

void TestEntity::init()
{
    // Initialisation des variables par défaut pour chaque test
    // Paramètres : (maxHp, speed, x, y, attackCooldown, attackDamage)
    archer = new ArcherModel(100, 5, 0, 0, 1.0f, 10);
    lancer = new LancerModel(150, 4, 10, 10, 1.5f, 20);
    knight = new KnightModel(200, 3, 20, 20, 2.0f, 30);

    board = new LeaderboardModel();

    game = new GameModel();
    game->resetGame(HeroType::Assassin);
    game->loadLevel(1);
    testEnemy1 = new LancerModel(25, 2, 0, 0, 1.0, 5);
    testEnemy2 = new LancerModel(25, 2, 0, 0, 1.0, 5);
}

void TestEntity::cleanup()
{
    // Nettoyage après chaque test pour éviter les fuites de mémoire
    delete archer;
    delete lancer;
    delete knight;

    archer = nullptr;
    lancer = nullptr;
    knight = nullptr;

    delete board;
    QFile::remove(realFilePath);

    delete testEnemy1;
    delete testEnemy2;
    delete game;
}

// ==========================================
// TESTS ARCHER
// ==========================================

void TestEntity::test_archer_move()
{
    QCOMPARE(archer->getHitBox().x(), 0.0);
    QCOMPARE(archer->getHitBox().y(), 0.0);

    archer->move(QPointF(50.5, 50.5));

    QCOMPARE(archer->getHitBox().x(), 50.5);
    QCOMPARE(archer->getHitBox().y(), 50.5);
}

void TestEntity::test_archer_takeDamage_and_death()
{
    // Utilisation de QSignalSpy pour vérifier que le signal 'death' est bien émis
    QSignalSpy spyDeath(archer, &ArcherModel::death);

    archer->takeDamage(50);
    QCOMPARE(spyDeath.count(), 0); // L'archer a 100 HP, 50 dégâts ne le tuent pas

    // L'archer a maintenant des I-frames (invincibilité). S'il prend des dégâts de suite, ils sont ignorés.
    archer->takeDamage(50);
    QCOMPARE(spyDeath.count(), 0); // Toujours vivant à cause des I-frames

    // Test de la mort sur une autre instance ou après avoir simulé le temps (ici on force juste les HP en recréant l'entité sans iframe)
    ArcherModel fatalArcher(100, 5, 0, 0, 1.0f, 10);
    QSignalSpy spyFatalDeath(&fatalArcher, &ArcherModel::death);
    fatalArcher.takeDamage(100);
    QCOMPARE(spyFatalDeath.count(), 1); // 100 dégâts d'un coup le tuent
}

void TestEntity::test_archer_handleAI()
{
    // L'archer doit fuir (renvoyer false) si le héros est à moins de 200 unités
    QPointF closeHeroPos(100, 0);
    bool readyToAttack = archer->handleAI(closeHeroPos);
    QCOMPARE(readyToAttack, false);

    // L'archer doit attaquer (renvoyer true) si le héros est à plus de 200 unités
    QPointF farHeroPos(300, 0);
    readyToAttack = archer->handleAI(farHeroPos);
    QCOMPARE(readyToAttack, true);
}

// ==========================================
// TESTS LANCER
// ==========================================

void TestEntity::test_lancer_move()
{
    lancer->move(QPointF(100, 150));
    QCOMPARE(lancer->getHitBox().x(), 100.0);
    QCOMPARE(lancer->getHitBox().y(), 150.0);
}

void TestEntity::test_lancer_takeDamage_and_death()
{
    QSignalSpy spyDeath(lancer, &LancerModel::death);

    // One-shot le Lancer (HP max = 150 défini dans init)
    lancer->takeDamage(150);

    QCOMPARE(spyDeath.count(), 1); // Le signal death doit être émis 1 fois
}

void TestEntity::test_lancer_handleAI()
{
    // Le Lancer avance vers le héros s'il est trop loin (renvoie false)
    QPointF farHeroPos(1000, 1000);
    bool readyToAttack = lancer->handleAI(farHeroPos);
    QCOMPARE(readyToAttack, false);

    // S'il est sur lui (ou très proche), il doit attaquer (renvoie true)
    QPointF closeHeroPos(lancer->getHitBox().center());
    readyToAttack = lancer->handleAI(closeHeroPos);
    QCOMPARE(readyToAttack, true);
}

// ==========================================
// TESTS KNIGHT
// ==========================================

void TestEntity::test_knight_move()
{
    knight->move(QPointF(200, 300));
    QCOMPARE(knight->getHitBox().x(), 200.0);
    QCOMPARE(knight->getHitBox().y(), 300.0);
}

void TestEntity::test_knight_takeDamage_and_death()
{
    QSignalSpy spyDeath(knight, &KnightModel::death);

    // Dégât non mortel (HP max = 200)
    knight->takeDamage(199);
    QCOMPARE(spyDeath.count(), 0);

    // Pour éviter le blocage I-frame dans le test, on teste la mort d'un coup
    KnightModel fatalKnight(200, 3, 20, 20, 2.0f, 30);
    QSignalSpy spyFatalDeath(&fatalKnight, &KnightModel::death);
    fatalKnight.takeDamage(200);
    QCOMPARE(spyFatalDeath.count(), 1);
}

void TestEntity::test_knight_handleAI()
{
    // Le Knight avance vers le héros s'il est trop loin (renvoie false)
    QPointF farHeroPos(1000, 1000);
    bool readyToAttack = knight->handleAI(farHeroPos);
    QCOMPARE(readyToAttack, false);

    // S'il est assez proche (Hitbox), il arrête d'avancer et attaque (renvoie true)
    QPointF closeHeroPos(knight->getHitBox().center());
    readyToAttack = knight->handleAI(closeHeroPos);
    QCOMPARE(readyToAttack, true);
}

void TestEntity::initTestCase() {
    if (QFile::exists(realFilePath)) {
        QFile::rename(realFilePath, backupFilePath);
    }
}

void TestEntity::cleanupTestCase() {
    if (QFile::exists(realFilePath)) {
        QFile::remove(realFilePath);
    }
    if (QFile::exists(backupFilePath)) {
        QFile::rename(backupFilePath, realFilePath);
    }
    qDebug() << "Tests termines. Vraie sauvegarde restauree avec succes !";
}

void TestEntity::testEmptyBoard() {
    QVERIFY(board->getTopScores().isEmpty());
}

void TestEntity::testAddSingleScore() {
    board->addScore("Antoine", 500);
    QCOMPARE(board->getTopScores().size(), 1);
    QCOMPARE(board->getTopScores().first().playerName, QString("Antoine"));
}

// --- EDGE CASE ---
void TestEntity::testSortingScores() {
    board->addScore("Antoine", 300);
    board->addScore("Arthur", 800);
    board->addScore("Quentin", 100);

    QCOMPARE(board->getTopScores().size(), 3);

    QCOMPARE(board->getTopScores().at(0).playerName, QString("Arthur"));
    QCOMPARE(board->getTopScores().at(0).score, 800);

    QCOMPARE(board->getTopScores().at(2).playerName, QString("Quentin"));
}

// Overflow test (Maximum array capacity)
void TestEntity::testMaxCapacity() {
    // We simulate 15 played games
    for(int i = 0; i < 15; i++) {
        board->addScore("Joueur", 100 + i);
    }

    int maxScoresLimit = 10;
    QVERIFY(board->getTopScores().size() <= maxScoresLimit);
}

// Testing for extreme or empty data
void TestEntity::testEmptyNameAndZeroScore() {
    board->addScore("", 0);

    QCOMPARE(board->getTopScores().size(), 1);
    QCOMPARE(board->getTopScores().first().playerName, QString(""));
    QCOMPARE(board->getTopScores().first().score, 0);
}


void TestEntity::testMenuInitialization() {
    QVERIFY(menu != nullptr);

    QList<QPushButton*> buttons = menu->findChildren<QPushButton*>();
    QCOMPARE(buttons.size(), 4);
}

void TestEntity::testMenuPlaySignalWithoutSpy() {
    bool playSignalEmitted = false;

    connect(menu, &MenuView::playRequested, this, [&]() {
        playSignalEmitted = true;
    });

    QList<QPushButton*> buttons = menu->findChildren<QPushButton*>();
    QPushButton* playButton = nullptr;
    for (QPushButton* btn : buttons) {
        if (btn->text() == LABEL_PLAY) {
            playButton = btn;
            break;
        }
    }

    QVERIFY(playButton != nullptr);

    // On simule le clic physique
    QTest::mouseClick(playButton, Qt::LeftButton);
    QVERIFY(playSignalEmitted == true);
}


// --- TESTS SÉLECTION DE PERSONNAGES ---

void TestEntity::testCharSelectInitialization() {
    QVERIFY(charSelect != nullptr);

    QList<QPushButton*> buttons = charSelect->findChildren<QPushButton*>();
    QCOMPARE(buttons.size(), 4);
}

void TestEntity::testCarouselWrapAroundRight() {
    QPushButton* rightArrow = nullptr;
    for (QPushButton* btn : charSelect->findChildren<QPushButton*>()) {
        if (btn->text() == ">") rightArrow = btn;
    }
    QVERIFY(rightArrow != nullptr);

    QPushButton* heroCard = nullptr;
    for (QPushButton* btn : charSelect->findChildren<QPushButton*>()) {
        if (btn->text().isEmpty()) heroCard = btn;
    }
    QVERIFY(heroCard != nullptr);

    HeroType selectedHero = HeroType::None;
    connect(charSelect, &CharSelectView::selectCharacterRequested, this, [&](HeroType type) {
        selectedHero = type;
    });

    // Forgeron par défaut
    QTest::mouseClick(heroCard, Qt::LeftButton);
    QCOMPARE(selectedHero, HeroType::Blacksmith);

    // Trois clics à droite (Débordement complet)
    QTest::mouseClick(rightArrow, Qt::LeftButton);
    QTest::mouseClick(rightArrow, Qt::LeftButton);
    QTest::mouseClick(rightArrow, Qt::LeftButton);

    // On doit être revenu au Forgeron
    QTest::mouseClick(heroCard, Qt::LeftButton);
    QCOMPARE(selectedHero, HeroType::Blacksmith);
}

void TestEntity::testCarouselWrapAroundLeft() {
    QPushButton* leftArrow = nullptr;
    for (QPushButton* btn : charSelect->findChildren<QPushButton*>()) {
        if (btn->text() == "<") leftArrow = btn;
    }
    QVERIFY(leftArrow != nullptr);

    QPushButton* heroCard = nullptr;
    for (QPushButton* btn : charSelect->findChildren<QPushButton*>()) {
        if (btn->text().isEmpty()) heroCard = btn;
    }

    HeroType selectedHero = HeroType::None;
    connect(charSelect, &CharSelectView::selectCharacterRequested, this, [&](HeroType type) {
        selectedHero = type;
    });

    // lic à gauche depuis l'index 0 (Forgeron)
    QTest::mouseClick(leftArrow, Qt::LeftButton);
    QTest::mouseClick(heroCard, Qt::LeftButton);

    // On doit avoir atterri sur l'Assassin
    QCOMPARE(selectedHero, HeroType::Assassin);
}

// ---------------------------------------------------------
// TESTS AUX LIMITES (Murs de la salle)
// ---------------------------------------------------------

void TestEntity::test_collision_mur_limite()
{
    // On récupère les limites de la première salle (le Spawn)
    QRectF roomBounds = game->getAllRooms().first()->getBounds();

    // On place l'ennemi DÉLIBÉRÉMENT hors des limites à gauche
    float outOfBoundsX = roomBounds.left() - 50;
    float validY = roomBounds.center().y();
    testEnemy1->move(QPointF(outOfBoundsX, validY));

    // Exécution
    // On fait appel à la fonction de collision
    game->collisionEnemy(testEnemy1);

    // Vérification
    // L'ennemi a dû être repoussé exactement sur le bord gauche de la salle
    float newX = testEnemy1->getHitBox().left();

    // QCOMPARE vérifie que la valeur actuelle correspond à la valeur attendue
    QCOMPARE(newX, roomBounds.left());
}

// ---------------------------------------------------------
// TESTS COMPLEXES (Chevauchement d'entités)
// ---------------------------------------------------------

void TestEntity::test_collision_entre_ennemis()
{
    // On place deux ennemis exactement au même endroit (Cas extrême de chevauchement)
    QPointF conflitPos(300, 300);
    testEnemy1->move(conflitPos);
    testEnemy2->move(conflitPos);

    // Exécution
    game->collisionEnemy(testEnemy1);

    // Vérification
    // Après collision, testEnemy1 DOIT avoir bougé pour ne plus chevaucher testEnemy2
    QRectF box1 = testEnemy1->getHitBox();
    QRectF box2 = testEnemy2->getHitBox();

    // QVERIFY évalue une condition booléenne
    // On vérifie que les deux boites ne s'intersectent plus (ou au maximum sur les bords)
    QVERIFY(box1.intersects(box2) == false);
}

// ---------------------------------------------------------
// TESTS NOMINAUX ET LOGIQUE MÉTIER (Smart Sliding)
// ---------------------------------------------------------

void TestEntity::test_smart_sliding_obstacle()
{
    // On place le joueur en bas
    game->getItsPlayer()->move(QPointF(300, 500));

    // On crée un obstacle au milieu (300, 300)
    RoomModel* spawnRoom = game->getAllRooms().first();
    spawnRoom->addObstacle(300, 300, 50, 50);

    // On place l'ennemi juste au-dessus de l'obstacle, essayant de descendre vers le joueur
    testEnemy1->move(QPointF(300, 290));

    // Exécution
    game->collisionEnemy(testEnemy1);

    // Vérification
    // Selon le code "Smart Sliding", si l'ennemi tape le haut de l'obstacle,
    // il doit glisser horizontalement vers le joueur (ici le joueur est aligné en X, donc le glissement dépend de ta logique exacte, mais il ne doit PAS traverser l'obstacle).

    QRectF globalObsRect = spawnRoom->getItsObstacles().first()->getHitBox();
    globalObsRect.translate(spawnRoom->getBounds().x(), spawnRoom->getBounds().y());

    // On vérifie que l'ennemi n'a pas traversé l'obstacle vers le bas
    QVERIFY(testEnemy1->getHitBox().bottom() <= globalObsRect.top());
}

QTEST_APPLESS_MAIN(TestEntity)

#include "tst_testentity.moc"
