#include <QtTest>
#include <QPushButton>
#include <QLabel>
#include "menuview.h"
#include "charselectview.h"
#include "config.h"

// ==========================================
// DÉCLARATION DE LA CLASSE
// ==========================================
class TestMenus : public QObject
{
    Q_OBJECT

public:
    TestMenus();
    ~TestMenus();

private slots:
    void init();
    void cleanup();
    void testMenuInitialization();
    void testMenuPlaySignalWithoutSpy();

    void testCharSelectInitialization();
    void testCarouselWrapAroundRight();
    void testCarouselWrapAroundLeft();

private:
    MenuView* menu;
    CharSelectView* charSelect;
};

TestMenus::TestMenus() {}
TestMenus::~TestMenus() {}

void TestMenus::init() {
    menu = new MenuView();
    charSelect = new CharSelectView();
}

void TestMenus::cleanup() {
    delete menu;
    delete charSelect;
    menu = nullptr;
    charSelect = nullptr;
}


// --- TESTS MENU PRINCIPAL ---

void TestMenus::testMenuInitialization() {
    QVERIFY(menu != nullptr);

    QList<QPushButton*> buttons = menu->findChildren<QPushButton*>();
    QCOMPARE(buttons.size(), 4);
}

void TestMenus::testMenuPlaySignalWithoutSpy() {
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

void TestMenus::testCharSelectInitialization() {
    QVERIFY(charSelect != nullptr);

    QList<QPushButton*> buttons = charSelect->findChildren<QPushButton*>();
    QCOMPARE(buttons.size(), 4);
}

void TestMenus::testCarouselWrapAroundRight() {
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

void TestMenus::testCarouselWrapAroundLeft() {
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

QTEST_MAIN(TestMenus)

#include "tst_testmenus.moc"
