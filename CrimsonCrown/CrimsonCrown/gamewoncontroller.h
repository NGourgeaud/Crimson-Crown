#ifndef GAMEWONCONTROLLER_H
#define GAMEWONCONTROLLER_H

#include <QObject>
#include "gamewonview.h"

class GameWonController : public QObject {
    Q_OBJECT
public:
    explicit GameWonController(QWidget *parentWidget = nullptr, QObject *parent = nullptr);

    QWidget *createView(int score);
    void destroyView();

signals:
    void backToMenuRequested();

private:
    GameWonView *itsView;
    QWidget *parentWidget;
};

#endif // GAMEWONCONTROLLER_H
