#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include <QObject>
#include "menuview.h"

class MenuController : public QObject {
    Q_OBJECT
public:
    explicit MenuController(QWidget *parentWidget = nullptr, QObject *parent = nullptr);
    QWidget *createView();
    void destroyView();

signals:
    void playRequested();
    void leaderboardRequested();
    void settingsRequested();
    void quitRequested();

private:
    MenuView *itsView;
    QWidget *parentWidget;
};

#endif // MENUCONTROLLER_H
