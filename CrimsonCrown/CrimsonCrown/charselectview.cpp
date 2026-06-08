#include "charselectview.h"
#include "config.h"

///
/// \brief CharSelectView::CharSelectView
/// \details Builds the character selection screen layout using nested layouts
/// to position the hero on the bridge and the return button at the bottom right.
///
CharSelectView::CharSelectView(QWidget *parent)
    : QWidget(parent) {

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(LAYOUT_MARGIN, LAYOUT_MARGIN, LAYOUT_MARGIN, LAYOUT_MARGIN);
    mainLayout->setAlignment(Qt::AlignTop);

    // Screen Title
    itsTitle = new QLabel(LABEL_CHARSELECT_TITLE);
    QFont titleFont("Alagard");
    titleFont.setPointSize(MENU_TITLE_FONT_SIZE);
    titleFont.setBold(true);
    itsTitle->setFont(titleFont);
    itsTitle->setStyleSheet("color: white; background: transparent;");
    mainLayout->addWidget(itsTitle, 0, Qt::AlignHCenter);

    mainLayout->addStretch(3);

    itsBlacksmithCardButton = new QPushButton();
    itsBlacksmithCardButton->setFixedSize(160, 200);

    QIcon blacksmithIcon(":/forgeron.png");
    itsBlacksmithCardButton->setIcon(blacksmithIcon);
    itsBlacksmithCardButton->setIconSize(QSize(160, 200));

    QString blacksmithCardStyle =
        "QPushButton {"
        "   border: none;"
        "   background-color: transparent;"
        "   background: none;"
        "   outline: none;"
        "}"
        "QPushButton:hover {"
        "   border: 3px solid #E30000;"
        "}";
    itsBlacksmithCardButton->setStyleSheet(blacksmithCardStyle);

    connect(itsBlacksmithCardButton, &QPushButton::clicked, this, [this]() {
        emit selectCharacterRequested(HeroType::Hunter);
    });
    mainLayout->addWidget(itsBlacksmithCardButton, 0, Qt::AlignHCenter);

    mainLayout->addSpacing(5);

    itsBlacksmithStatsLabel = new QLabel();
    itsBlacksmithStatsLabel->setFixedSize(600, 130);
    itsBlacksmithStatsLabel->setText(STATS_BLACKSMITH);
    itsBlacksmithStatsLabel->setAlignment(Qt::AlignCenter);
    itsBlacksmithStatsLabel->setStyleSheet("background: transparent;");
    mainLayout->addWidget(itsBlacksmithStatsLabel, 0, Qt::AlignHCenter);

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
    connect(itsBackButton, &QPushButton::clicked, this, &CharSelectView::backRequested);

    bottomLayout->addWidget(itsBackButton);

    mainLayout->addLayout(bottomLayout);
}

///
/// \brief CharSelectView::paintEvent
///
void CharSelectView::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap background(":/ecran_selection_personnages");
    painter.drawPixmap(0, 0, this->width(), this->height(), background);
}
