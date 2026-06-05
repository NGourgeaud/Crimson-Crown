#ifndef CHARSELECTCONTROLLER_H
#define CHARSELECTCONTROLLER_H

#include <QObject>
#include "charselectview.h"


class CharSelectController : public QObject {
    Q_OBJECT
public:
    explicit CharSelectController(QWidget *parentWidget = nullptr, QObject *parent = nullptr);

    QWidget *createView();
    void destroyView();

signals:
    void characterSelected(HeroType type);
    void backToMenuRequested();

private:
    CharSelectView *itsView;
    QWidget *parentWidget;
};

#endif // CHARSELECTCONTROLLER_H
