// File Name: leaderboardview.cpp
// Group: A11
// Description: View class rendering the leaderboard UI, displaying top scores in a formatted Qt table.

#include "leaderboardview.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QPainter>

///
/// \brief Constructs the Leaderboard UI, setting up the table widget and formatting the loaded scores.
/// \param parent The parent QWidget.
/// \param scores The list of top ScoreEntry objects to display.
///
LeaderboardView::LeaderboardView(QWidget *parent, const QList<ScoreEntry>& scores)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(50, 40, 50, 40);

    // TITLE
    QLabel* title = new QLabel("CLASSEMENT GENERAL", this);
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont("Alagard", 38));
    title->setStyleSheet("color: #FFFFFF; margin-bottom: 25px; font-weight: bold;");
    layout->addWidget(title);

    // BOARD CONFIGURATION
    table = new QTableWidget(10, 3, this);

    QTableWidgetItem* headerRank = new QTableWidgetItem("Rang");
    headerRank->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    table->setHorizontalHeaderItem(0, headerRank);

    QTableWidgetItem* headerScore = new QTableWidgetItem("Meilleur score");
    headerScore->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    table->setHorizontalHeaderItem(1, headerScore);

    QTableWidgetItem* headerName = new QTableWidgetItem("Nom");
    headerName->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    table->setHorizontalHeaderItem(2, headerName);

    table->verticalHeader()->setVisible(false);
    table->setShowGrid(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setFixedSize(700, 450);

    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->setColumnWidth(0, 150);
    table->setColumnWidth(1, 250);

    table->setStyleSheet(
        "QHeaderView::section { "
        "   background-color: transparent; "
        "   color: #333333; " // Change to white (#FFFFFF) if your background is very dark
        "   font-family: Alagard; "
        "   font-size: 24px; "
        "   font-weight: bold; "
        "   border: none; "
        "   padding-bottom: 10px; "
        "   padding-left: 15px; "
        "   padding-right: 15px; "
        "}"
        "QTableWidget { "
        "   background-color: #222222; "
        "   color: #FFFFFF; "
        "   font-family: Alagard; "
        "   font-size: 22px; "
        "   border: none; "
        "   border-radius: 6px; "
        "   padding: 15px; "
        "}"
        );

    // DYNAMIC LINES FILLING
    for (int i = 0; i < 10; ++i) {
        QString rankText = "";
        if (i == 0)      rankText = "🥇";
        else if (i == 1) rankText = "🥈";
        else if (i == 2) rankText = "🥉";
        else             rankText = " " + QString::number(i + 1);

        if (i < scores.size()) {
            QTableWidgetItem* rankItem = new QTableWidgetItem(rankText);
            rankItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            table->setItem(i, 0, rankItem);

            QTableWidgetItem* scoreItem = new QTableWidgetItem(QString::number(scores[i].score));
            scoreItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            table->setItem(i, 1, scoreItem);

            QTableWidgetItem* nameItem = new QTableWidgetItem(scores[i].playerName);
            nameItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            table->setItem(i, 2, nameItem);
        } else {
            QTableWidgetItem* rankItem = new QTableWidgetItem(rankText);
            rankItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            table->setItem(i, 0, rankItem);

            QTableWidgetItem* scoreItem = new QTableWidgetItem("---");
            scoreItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            table->setItem(i, 1, scoreItem);

            QTableWidgetItem* nameItem = new QTableWidgetItem("---");
            nameItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            table->setItem(i, 2, nameItem);
        }
    }

    layout->addWidget(table);
    layout->addSpacing(20);

    // BACK BUTTON
    backButton = new QPushButton("RETOUR AU MENU", this);
    backButton->setFixedSize(280, 50);
    backButton->setFont(QFont("Alagard", 16));
    backButton->setStyleSheet(
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
        "}"
        );
    layout->addWidget(backButton, 0, Qt::AlignCenter);

    connect(backButton, &QPushButton::clicked, this, &LeaderboardView::backRequested);
}

///
/// \brief Handles the background rendering event for the Leaderboard screen.
/// \param event The paint event triggered by the Qt framework.
///
void LeaderboardView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    QPixmap background(":/fondjeu.png");

    if (!background.isNull()) {
        painter.drawPixmap(this->rect(), background);
    }
}
