#ifndef CHARSELECTVIEW_H
#define CHARSELECTVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include "config.h"

class CharSelectView : public QWidget {
    Q_OBJECT
public:
    explicit CharSelectView(QWidget *parent = nullptr);

signals:
    void selectCharacterRequested(HeroType hero);
    void backRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *itsTitle;

    // Éléments du Carrousel
    QPushButton *itsLeftArrow;
    QPushButton *itsRightArrow;
    QPushButton *itsHeroCardButton; // Le bouton central (l'image du perso)
    QLabel *itsHeroStatsLabel;      // Les stats du perso affiché

    QPushButton *itsBackButton;

    // Logique du Carrousel
    int currentHeroIndex; // 0 = Forgeron, 1 = Chasseur, 2 = Assassin
    void updateCarousel(); // Fonction pour rafraîchir l'affichage
};

#endif // CHARSELECTVIEW_H
