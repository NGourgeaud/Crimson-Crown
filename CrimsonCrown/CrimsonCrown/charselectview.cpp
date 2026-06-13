// File Name: charselectview.cpp
// Group: A11
// Description: View class for the character selection screen, rendering the hero carousel, statistics, and UI buttons.

#include "charselectview.h"
#include "config.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPainter>

///
/// \brief Constructs the UI layout, styling, and widget connections for the character selection screen.
/// \param parent The parent QWidget to attach this view to.
///
CharSelectView::CharSelectView(QWidget *parent)
    : QWidget(parent), currentHeroIndex(0) {

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 60, 0, 10);

    // TITLE SECTION
    itsTitle = new QLabel(LABEL_CHARSELECT_TITLE, this);
    QFont titleFont("Alagard");
    titleFont.setPointSize(MENU_TITLE_FONT_SIZE);
    titleFont.setBold(true);
    itsTitle->setFont(titleFont);
    itsTitle->setStyleSheet("color: white; background: transparent;");
    itsTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(itsTitle);

    mainLayout->addStretch(1);

    // CAROUSEL SELECTION SECTION
    QHBoxLayout *selectionLayout = new QHBoxLayout();
    selectionLayout->setContentsMargins(50, 0, 50, 0);

    QString arrowStyle =
        "QPushButton { background-color: rgba(0, 0, 0, 150); color: white; "
        "font-family: 'Alagard'; font-size: 30px; font-weight: bold; "
        "border: 2px solid #555555; border-radius: 5px; }"
        "QPushButton:hover { background-color: #E30000; border-color: white; }";

    // Left navigation arrow setup
    itsLeftArrow = new QPushButton("<", this);
    itsLeftArrow->setStyleSheet(arrowStyle);
    itsLeftArrow->setFixedSize(60, 60);
    connect(itsLeftArrow, &QPushButton::clicked, this, [this]() {
        currentHeroIndex--;
        if (currentHeroIndex < 0) currentHeroIndex = 2;
        updateCarousel();
    });
    selectionLayout->addWidget(itsLeftArrow, 0, Qt::AlignVCenter);

    selectionLayout->addStretch();

    QString cardStyle =
        "QPushButton { border: none; background-color: transparent; background: none; outline: none; }"
        "QPushButton:hover { border: 3px solid #E30000; }";

    // Main hero portrait button that confirms selection when clicked
    itsHeroCardButton = new QPushButton(this);
    itsHeroCardButton->setStyleSheet(cardStyle);
    itsHeroCardButton->setFixedSize(160, 200);
    connect(itsHeroCardButton, &QPushButton::clicked, this, [this]() {
        if (currentHeroIndex == 0) {
            emit selectCharacterRequested(HeroType::Blacksmith);
        } else if (currentHeroIndex == 1) {
            emit selectCharacterRequested(HeroType::Hunter);
        } else {
            emit selectCharacterRequested(HeroType::Assassin);
        }
    });
    selectionLayout->addWidget(itsHeroCardButton, 0, Qt::AlignBottom);
    selectionLayout->addStretch();

    // Right navigation arrow setup
    itsRightArrow = new QPushButton(">", this);
    itsRightArrow->setStyleSheet(arrowStyle);
    itsRightArrow->setFixedSize(60, 60);
    connect(itsRightArrow, &QPushButton::clicked, this, [this]() {
        currentHeroIndex++;
        if (currentHeroIndex > 2) currentHeroIndex = 0;
        updateCarousel();
    });
    selectionLayout->addWidget(itsRightArrow, 0, Qt::AlignVCenter);

    mainLayout->addLayout(selectionLayout);

    // CHARACTER STATISTICS SECTION
    itsHeroStatsLabel = new QLabel(this);
    itsHeroStatsLabel->setStyleSheet(
        "background-color: rgba(0, 0, 0, 160); "
        "color: white; "
        "font-family: 'Alagard'; "
        "font-size: 16px; "
        "padding: 5px; "
        "border-radius: 8px;"
        );
    itsHeroStatsLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    itsHeroStatsLabel->setWordWrap(true);
    itsHeroStatsLabel->setFixedWidth(600);
    itsHeroStatsLabel->setMinimumHeight(110);

    QHBoxLayout *statsLayout = new QHBoxLayout();
    statsLayout->addWidget(itsHeroStatsLabel, 0, Qt::AlignCenter);
    mainLayout->addLayout(statsLayout);

    mainLayout->addSpacing(10);

    // BACK BUTTON SECTION
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();

    QString backButtonStyle =
        "QPushButton { background-color: #111111; color: white; border: 2px solid #555555; "
        "font-family: 'Alagard'; font-size: 15px; font-weight: bold; border-radius: 3px; }"
        "QPushButton:hover { background-color: #E30000; border-color: white; }";

    itsBackButton = new QPushButton(LABEL_BACK, this);
    itsBackButton->setStyleSheet(backButtonStyle);
    itsBackButton->setFixedSize(220, BUTTON_HEIGHT);
    connect(itsBackButton, &QPushButton::clicked, this, &CharSelectView::backRequested);

    bottomLayout->addWidget(itsBackButton);
    bottomLayout->setContentsMargins(0, 0, 40, 20);

    mainLayout->addLayout(bottomLayout);

    // Initialize the carousel state to the first hero
    updateCarousel();
}

///
/// \brief Updates the visual assets and text statistics based on the currently selected hero index.
///
void CharSelectView::updateCarousel() {
    if (currentHeroIndex == 0) {
        itsHeroCardButton->setIcon(QIcon(":/forgeron.png"));
        itsHeroCardButton->setIconSize(QSize(160, 200));
        itsHeroStatsLabel->setText(STATS_BLACKSMITH);
    } else if (currentHeroIndex == 1) {
        itsHeroCardButton->setIcon(QIcon(":/chasseur.png"));
        itsHeroCardButton->setIconSize(QSize(160, 200));
        itsHeroStatsLabel->setText(STATS_HUNTER);
    } else {
        itsHeroCardButton->setIcon(QIcon(":/assassin.png"));
        itsHeroCardButton->setIconSize(QSize(160, 200));
        itsHeroStatsLabel->setText(STATS_ASSASSIN);
    }
}

///
/// \brief Handles the background rendering event for the character selection screen.
/// \param event The paint event triggered by the Qt framework.
///
void CharSelectView::paintEvent(QPaintEvent *event) {
    // Acknowledges the event without using it directly to avoid compiler warnings
    Q_UNUSED(event);

    QPainter painter(this);
    QPixmap background(":/ecran_selection_personnages");
    painter.drawPixmap(0, 0, this->width(), this->height(), background);
}
