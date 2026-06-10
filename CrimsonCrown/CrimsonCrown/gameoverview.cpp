#include "gameoverview.h"

GameOverView::GameOverView(int score, HeroType hero, QWidget *parent)
    : QWidget(parent) {

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(LAYOUT_MARGIN, LAYOUT_MARGIN, LAYOUT_MARGIN, LAYOUT_MARGIN);

    // 1. Remplacer addStretch(7) par un espacement fixe en pixels
    // Augmente cette valeur pour descendre le perso, diminue-la pour le monter !
    mainLayout->addSpacing(380);

    // 2. CHOIX DYNAMIQUE DU PERSONNAGE (DÉFAITE)
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
    mainLayout->addWidget(characterImage, 0, Qt::AlignHCenter); // Conserve le centrage automatique !

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
