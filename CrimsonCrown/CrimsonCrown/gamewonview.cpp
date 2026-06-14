// File Name: gamewonview.cpp
// Group: A11
// Description: View class responsible for rendering the victory screen, displaying the final score and the victorious hero's sprite.

#include "gamewonview.h"

///
/// \brief Constructs the victory UI, displaying the final score, the winning hero sprite, and a return button.
/// \param score The final score to display.
/// \param hero The type of hero to determine which victorious sprite to load.
/// \param parent The parent QWidget.
///
GameWonView::GameWonView(int score, HeroType hero, QWidget *parent)
    : QWidget(parent) {

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(LAYOUT_MARGIN, LAYOUT_MARGIN, LAYOUT_MARGIN, LAYOUT_MARGIN);

    // 1. Fixed pixel spacing to align the feet on the bridge
    // Adjust this value (e.g., 380) to perfectly position the character
    mainLayout->addSpacing(380);

    // 2. DYNAMIC CHARACTER SELECTION (VICTORY SPRITE)
    QString imagePath;
    switch (hero) {
    case HeroType::Assassin: imagePath = ":/assassin-win.png"; break;
    case HeroType::Hunter:   imagePath = ":/chasseur-win.png"; break;
    default:                 imagePath = ":/forgeron-win.png"; break;
    }

    QLabel *characterImage = new QLabel();
    characterImage->setFixedSize(160, 200);
    QPixmap spriteVictoire(imagePath);
    characterImage->setPixmap(spriteVictoire.scaled(160, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    characterImage->setStyleSheet("background: transparent;");
    mainLayout->addWidget(characterImage, 0, Qt::AlignHCenter); // Maintains automatic centering!

    mainLayout->addSpacing(20);

    // 4. THE SCORE IN WHITE AT THE VERY BOTTOM OF THE BRIDGE
    QString textScore = QString("SCORE FINAL : %1").arg(score);
    itsScoreLabel = new QLabel(textScore);
    QFont scoreFont("Alagard");
    scoreFont.setPointSize(45);
    scoreFont.setBold(true);
    itsScoreLabel->setFont(scoreFont);
    itsScoreLabel->setStyleSheet("color: white; background: transparent;");
    mainLayout->addWidget(itsScoreLabel, 0, Qt::AlignHCenter);

    // 5. BACK BUTTON (Bottom right)
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch(1);

    QString backButtonStyle =
        "QPushButton {"
        "   background-color: #111111;"
        "   color: white;"
        "   border: 2px solid #555555;"
        "   font-family: 'Alagard';"
        "   font-size: 15px;"
        "   font-weight: bold;"
        "   border-radius: 3px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #FFD700;"
        "   color: black;"
        "   border-color: white;"
        "}";

    itsBackButton = new QPushButton(LABEL_BACK);
    itsBackButton->setFixedSize(220, BUTTON_HEIGHT);
    itsBackButton->setStyleSheet(backButtonStyle);
    connect(itsBackButton, &QPushButton::clicked, this, &GameWonView::menuRequested);

    bottomLayout->addWidget(itsBackButton);
    mainLayout->addLayout(bottomLayout);
}

///
/// \brief Handles the background rendering event for the victory screen.
/// \param event The paint event triggered by the Qt framework.
///
void GameWonView::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap background(":/ecran_victoire");

    if (!background.isNull()) {
        painter.drawPixmap(0, 0, this->width(), this->height(), background);
    } else {
        painter.fillRect(this->rect(), Qt::black);
    }
}
