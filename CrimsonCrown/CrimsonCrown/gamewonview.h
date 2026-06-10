#ifndef GAMEWONVIEW_H
#define GAMEWONVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include "config.h"

class GameWonView : public QWidget {
    Q_OBJECT
public:
    explicit GameWonView(int score, HeroType hero, QWidget *parent = nullptr);

signals:
    void menuRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *itsScoreLabel;
    QPushButton *itsBackButton;
};

#endif // GAMEWONVIEW_H
