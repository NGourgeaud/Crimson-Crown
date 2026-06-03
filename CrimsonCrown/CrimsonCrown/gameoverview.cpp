#include "gameoverview.h"

GameOverView::GameOverView(int score, QWidget *parent)
    : QWidget(parent) {

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(LAYOUT_MARGIN, LAYOUT_MARGIN, LAYOUT_MARGIN, LAYOUT_MARGIN);
    mainLayout->setAlignment(Qt::AlignTop);

    // Titre de l'écran
    itsTitle = new QLabel("GAME OVER");
    QFont titleFont("Alagard");
    titleFont.setPointSize(MENU_TITLE_FONT_SIZE + 10);
    titleFont.setBold(true);
    itsTitle->setFont(titleFont);
    itsTitle->setStyleSheet("color: #E30000; background: transparent;"); // Rouge pour la défaite
    mainLayout->addWidget(itsTitle, 0, Qt::AlignHCenter);

    mainLayout->addSpacing(20);

    // Affichage du score
    QString textScore = "Score : " + QString::number(score);
    itsScoreLabel = new QLabel(textScore);
    QFont scoreFont("Alagard");
    scoreFont.setPointSize(MENU_TITLE_FONT_SIZE - 10);
    itsScoreLabel->setFont(scoreFont);
    itsScoreLabel->setStyleSheet("color: white; background: transparent;");
    mainLayout->addWidget(itsScoreLabel, 0, Qt::AlignHCenter);

    mainLayout->addStretch(1);

    // Layout du bas pour le bouton retour
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

    itsBackButton = new QPushButton(LABEL_BACK); // Utilise ta macro de config.h
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
        painter.fillRect(this->rect(), Qt::black); // Sécurité si l'image manque
    }
}
