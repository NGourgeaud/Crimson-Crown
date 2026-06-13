// File Name: gameoverview.cpp
// Group: A11
// Description: View class responsible for rendering the Game Over screen, displaying the final score and the defeated hero's sprite.

#include "gameoverview.h"

///
/// \brief Constructs the Game Over UI, displaying the final score, the defeated hero sprite, and a return button.
/// \param score The final score to display.
/// \param hero The type of hero to determine which defeated sprite to load.
/// \param parent The parent QWidget.
///
GameOverView::GameOverView(int score, HeroType hero, QWidget *parent)
    : QWidget(parent) {

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(LAYOUT_MARGIN, LAYOUT_MARGIN, LAYOUT_MARGIN, LAYOUT_MARGIN);

    // 1. Fixed spacing to position the character correctly on the background bridge
    // Increase this value to move the character down, decrease it to move them up
    mainLayout->addSpacing(380);

    // 2. DYNAMIC CHARACTER SELECTION (DEFEAT SPRITE)
    QString imagePath;
    switch (hero) {
    case HeroType::Assassin: imagePath = ":/assassin-loose.png"; break;
    case HeroType::Hunter:   imagePath = ":/chasseur-loose.png"; break;
    default:                 imagePath = ":/forgeron-loose.png"; break;
    }

    QLabel *characterImage = new QLabel();
    characterImage->setFixedSize(160, 200);
    QPixmap spriteDefaite(imagePath);
    characterImage->setPixmap(spriteDefaite.scaled(160, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    characterImage->setStyleSheet("background: transparent;");
    mainLayout->addWidget(characterImage, 0, Qt::AlignHCenter); // Maintains automatic horizontal centering

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

    mainLayout->addStretch(1);

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
        "   background-color: #E30000;"
        "   border-color: white;"
        "}";

    itsBackButton = new QPushButton(LABEL_BACK);
    itsBackButton->setFixedSize(220, BUTTON_HEIGHT);
    itsBackButton->setStyleSheet(backButtonStyle);
    connect(itsBackButton, &QPushButton::clicked, this, &GameOverView::menuRequested);

    bottomLayout->addWidget(itsBackButton);
    mainLayout->addLayout(bottomLayout);
}

///
/// \brief Handles the background rendering event for the Game Over screen.
/// \param event The paint event triggered by the Qt framework.
///
void GameOverView::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap background(":/ecran_defaite");

    if (!background.isNull()) {
        painter.drawPixmap(0, 0, this->width(), this->height(), background);
    } else {
        painter.fillRect(this->rect(), Qt::black);
    }
}
