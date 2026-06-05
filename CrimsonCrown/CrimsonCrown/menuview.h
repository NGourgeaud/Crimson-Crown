#ifndef MENUVIEW_H
#define MENUVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>

class MenuView : public QWidget {
    Q_OBJECT
public:
    explicit MenuView(QWidget *parent = nullptr);

signals:
    void playRequested();
    void leaderboardRequested();
    void settingsRequested();
    void quitRequested();

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // MENUVIEW_H
