#include "charselectview.h"
#include "config.h"

///
/// \brief CharSelectView::CharSelectView
/// \details Constructor for the character selection view.
/// Coordinates a centered layout system, applies a dark retro arcade stylesheet
/// onto UI buttons, and binds interaction signals to enforce the MVC pattern.
/// \param parent Pointer to the parent QWidget container (the MainWindow).
///
CharSelectView::CharSelectView(QWidget *parent)
    : QWidget(parent) {

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    mainLayout->setContentsMargins(LAYOUT_MARGIN, LAYOUT_MARGIN, LAYOUT_MARGIN, LAYOUT_MARGIN);

    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->setAlignment(Qt::AlignCenter);

    itsTitle = new QLabel(LABEL_CHARSELECT_TITLE);
    QFont titleFont("Alagard");
    titleFont.setPointSize(MENU_TITLE_FONT_SIZE);
    titleFont.setBold(true);
    itsTitle->setFont(titleFont);
    itsTitle->setStyleSheet("color: white; background: transparent;");
    centerLayout->addWidget(itsTitle, 0, Qt::AlignHCenter);

    centerLayout->addStretch(4);

    QString buttonStyle =
        "QPushButton {"
        "   background-color: black;"
        "   color: white;"
        "   border: 2px solid #555555;"
        "   font-family: 'Alagard';"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #222222;"
        "   border-color: white;"
        "}";

    itsBlacksmithCardButton = new QPushButton(LABEL_CONTINUE);
    itsBlacksmithCardButton->setFixedSize(220, BUTTON_HEIGHT);
    itsBlacksmithCardButton->setStyleSheet(buttonStyle);
    connect(itsBlacksmithCardButton, &QPushButton::clicked, this, &CharSelectView::selectCharacterRequested);
    centerLayout->addWidget(itsBlacksmithCardButton, 0, Qt::AlignHCenter);

    centerLayout->addSpacing(LAYOUT_SPACING);

    itsBackButton = new QPushButton(LABEL_BACK);
    itsBackButton->setFixedSize(220, BUTTON_HEIGHT);
    itsBackButton->setStyleSheet(buttonStyle);
    connect(itsBackButton, &QPushButton::clicked, this, &CharSelectView::backRequested);
    centerLayout->addWidget(itsBackButton, 0, Qt::AlignHCenter);

    centerLayout->addStretch(1);

    mainLayout->addLayout(centerLayout);
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
