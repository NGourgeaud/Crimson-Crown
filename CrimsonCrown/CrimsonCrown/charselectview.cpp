#include "charselectview.h"
#include "config.h"

CharSelectView::CharSelectView(QWidget *parent)
    : QWidget(parent), currentHeroIndex(0) {

    // SCREEB TITLE
    itsTitle = new QLabel(LABEL_CHARSELECT_TITLE, this);
    QFont titleFont("Alagard");
    titleFont.setPointSize(MENU_TITLE_FONT_SIZE);
    titleFont.setBold(true);
    itsTitle->setFont(titleFont);
    itsTitle->setStyleSheet("color: white; background: transparent;");
    itsTitle->setAlignment(Qt::AlignCenter);
    itsTitle->setGeometry(0, 60, WINDOW_WIDTH, 50);

    // --- STYLES ---
    QString arrowStyle =
        "QPushButton { background-color: rgba(0, 0, 0, 150); color: white; "
        "font-family: 'Alagard'; font-size: 30px; font-weight: bold; "
        "border: 2px solid #555555; border-radius: 5px; }"
        "QPushButton:hover { background-color: #E30000; border-color: white; }";

    QString cardStyle =
        "QPushButton { border: none; background-color: transparent; background: none; outline: none; }"
        "QPushButton:hover { border: 3px solid #E30000; }";

    itsLeftArrow = new QPushButton("<", this);
    itsLeftArrow->setStyleSheet(arrowStyle);
    itsLeftArrow->setGeometry(40, 370, 60, 60);
    connect(itsLeftArrow, &QPushButton::clicked, this, [this]() {
        currentHeroIndex--;
        if (currentHeroIndex < 0) currentHeroIndex = 1;
        updateCarousel();
    });

    itsRightArrow = new QPushButton(">", this);
    itsRightArrow->setStyleSheet(arrowStyle);
    itsRightArrow->setGeometry(1100, 370, 60, 60);
    connect(itsRightArrow, &QPushButton::clicked, this, [this]() {
        currentHeroIndex++;
        if (currentHeroIndex > 1) currentHeroIndex = 0;
        updateCarousel();
    });

    // THE CHARACTER (Goes down onto the deck)
    itsHeroCardButton = new QPushButton(this);
    itsHeroCardButton->setStyleSheet(cardStyle);
    itsHeroCardButton->setGeometry(520, 350, 160, 200);
    connect(itsHeroCardButton, &QPushButton::clicked, this, [this]() {
        if (currentHeroIndex == 0) {
            emit selectCharacterRequested(HeroType::Blacksmith);
        } else {
            emit selectCharacterRequested(HeroType::Hunter);
        }
    });

    // STATS
    itsHeroStatsLabel = new QLabel(this);
    itsHeroStatsLabel->setStyleSheet("background: transparent;");
    itsHeroStatsLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    itsHeroStatsLabel->setWordWrap(true);
    itsHeroStatsLabel->setGeometry(300, 560, 600, 150);

    // BACK TO MENU BUTTON
    QString backButtonStyle =
        "QPushButton { background-color: #111111; color: white; border: 2px solid #555555; "
        "font-family: 'Alagard'; font-size: 15px; font-weight: bold; border-radius: 3px; }"
        "QPushButton:hover { background-color: #E30000; border-color: white; }";

    itsBackButton = new QPushButton(LABEL_BACK, this);
    itsBackButton->setStyleSheet(backButtonStyle);
    itsBackButton->setGeometry(WINDOW_WIDTH - 260, WINDOW_HEIGHT - 90, 220, BUTTON_HEIGHT);
    connect(itsBackButton, &QPushButton::clicked, this, &CharSelectView::backRequested);

    // Initialisation
    updateCarousel();
}

void CharSelectView::updateCarousel() {
    if (currentHeroIndex == 0) {
        itsHeroCardButton->setIcon(QIcon(":/forgeron.png"));
        itsHeroCardButton->setIconSize(QSize(160, 200));
        itsHeroStatsLabel->setText(STATS_BLACKSMITH);
    } else if (currentHeroIndex == 1) {
        itsHeroCardButton->setIcon(QIcon(":/chasseur.png"));
        itsHeroCardButton->setIconSize(QSize(160, 200));
        itsHeroStatsLabel->setText(STATS_HUNTER);
    } /*else {
        itsHeroCardButton->setIcon(QIcon(":/assasin.png"));
        itsHeroCardButton->setIconSize(QSize(160, 200));
        itsHeroStatsLabel->setText(STATS_ASSASSIN);
    }*/
}

void CharSelectView::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap background(":/ecran_selection_personnages");
    painter.drawPixmap(0, 0, this->width(), this->height(), background);
}
