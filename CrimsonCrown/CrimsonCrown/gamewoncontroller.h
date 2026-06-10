#ifndef GAMEWONCONTROLLER_H
#define GAMEWONCONTROLLER_H

#include <QObject>
#include "gamewonview.h"

class GameWonController : public QObject {
    Q_OBJECT
public:
    explicit GameWonController(QWidget *parentWidget = nullptr, QObject *parent = nullptr);

    QWidget* createView(int score, HeroType hero);
    void destroyView();

signals:
    void menuRequested();

private:
    GameWonView *itsView;
    QWidget *parentWidget;
};

#endif // GAMEWONCONTROLLER_H
