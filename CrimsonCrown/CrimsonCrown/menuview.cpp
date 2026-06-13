// File Name: menuview.cpp
// Group: A11
// Description: View class responsible for rendering the main menu interface, including title, background, and navigation buttons.

#include "menuview.h"
#include "config.h"

///
/// \brief Constructor for the main menu view.
/// \details Coordinates a compact, left-aligned layout system, applies a custom dark arcade stylesheet (CSS) onto UI buttons, and configures the centralized title.
/// \param parent Pointer to the parent QWidget container.
///
MenuView::MenuView(QWidget *parent)
    : QWidget(parent) {

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(LAYOUT_MARGIN, LAYOUT_MARGIN,
                                   LAYOUT_MARGIN, LAYOUT_MARGIN);
    mainLayout->setSpacing(50);

    // ---------------- LEFT SIDE: BUTTONS ----------------
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    leftLayout->addStretch(4);

    QLabel *title = new QLabel(MENU_TITLE);
    QFont titleFont("Alagard");
    titleFont.setPointSize(MENU_TITLE_FONT_SIZE);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setStyleSheet("color: white; background: transparent;");

    leftLayout->addWidget(title, 0, Qt::AlignHCenter);
    leftLayout->addSpacing(LAYOUT_SPACING * 2);

    QString buttonStyle =
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

    QPushButton *playBtn = new QPushButton(LABEL_PLAY);
    playBtn->setFixedSize(220, BUTTON_HEIGHT);
    playBtn->setStyleSheet(buttonStyle);
    connect(playBtn, &QPushButton::clicked, this, &MenuView::playRequested);
    leftLayout->addWidget(playBtn, 0, Qt::AlignHCenter);

    leftLayout->addSpacing(10);

    QPushButton *leaderboardBtn = new QPushButton(LABEL_LEADERBOARD);
    leaderboardBtn->setFixedSize(220, BUTTON_HEIGHT);
    leaderboardBtn->setStyleSheet(buttonStyle);
    connect(leaderboardBtn, &QPushButton::clicked, this, &MenuView::leaderboardRequested);
    leftLayout->addWidget(leaderboardBtn, 0, Qt::AlignHCenter);

    leftLayout->addSpacing(10);

    QPushButton *settingsBtn = new QPushButton(LABEL_SETTINGS);
    settingsBtn->setFixedSize(220, BUTTON_HEIGHT);
    settingsBtn->setStyleSheet(buttonStyle);
    connect(settingsBtn, &QPushButton::clicked, this, &MenuView::settingsRequested);
    leftLayout->addWidget(settingsBtn, 0, Qt::AlignHCenter);
    leftLayout->addSpacing(10);

    QPushButton *quitBtn = new QPushButton(LABEL_QUIT);
    quitBtn->setFixedSize(220, BUTTON_HEIGHT);
    quitBtn->setStyleSheet(buttonStyle);
    connect(quitBtn, &QPushButton::clicked, this, &MenuView::quitRequested);
    leftLayout->addWidget(quitBtn, 0, Qt::AlignHCenter);

    leftLayout->addStretch(1);
    mainLayout->addLayout(leftLayout);
}

///
/// \brief Reimplementation of Qt's paint event to draw the menu backdrop.
/// \param event The paint event triggered by the Qt framework.
///
void MenuView::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap background(":/fondjeu.png");

    painter.drawPixmap(0, 0, this->width(), this->height(), background);
}
