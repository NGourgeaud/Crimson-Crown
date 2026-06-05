#ifndef GAMEOVERVIEW_H
#define GAMEOVERVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include "config.h"

class GameOverView : public QWidget {
    Q_OBJECT
public:
    explicit GameOverView(int score, QWidget *parent = nullptr);

signals:
    void menuRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *itsScoreLabel;
    QPushButton *itsBackButton;
};

#endif // GAMEOVERVIEW_H
