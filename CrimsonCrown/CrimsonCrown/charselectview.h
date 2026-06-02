#ifndef CHARSELECTVIEW_H
#define CHARSELECTVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>


class CharSelectView : public QWidget {
    Q_OBJECT
public:
    explicit CharSelectView(QWidget *parent = nullptr);

signals:
    void selectCharacterRequested();
    void backRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *itsTitle;
    QPushButton *itsBlacksmithCardButton;
    QPushButton *itsBackButton;
};

#endif // CHARSELECTVIEW_H
