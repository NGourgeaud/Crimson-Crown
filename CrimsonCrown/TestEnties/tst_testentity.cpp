#include <QTest>
#include <QSignalSpy>

// add necessary includes here
#include "../CrimsonCrown/archermodel.h"
#include "../CrimsonCrown/lancermodel.h"
#include "../CrimsonCrown/knightmodel.h"

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

private:
    // Variables par défaut gérées par init() et cleanup()
    ArcherModel* archer;
    LancerModel* lancer;
    KnightModel* knight;
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

QTEST_APPLESS_MAIN(TestEntity)

#include "tst_testentity.moc"
