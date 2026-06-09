#ifndef SETTINGVIEW_H
#define SETTINGVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSlider>
#include <QLabel>
#include <QMap>
#include <QPaintEvent>
#include <QPainter>
#include "settingmodel.h"

class SettingView : public QWidget
{
    Q_OBJECT
public:
    explicit SettingView(SettingModel *model, QWidget *parent = nullptr);
    void setupUI();
    void updateUIFromModel();

signals:
    void backRequested();
    void keyBindChangedRequested(const QString& action, Qt::Key key);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    SettingModel *itsSettingModel;

    QPushButton * boutonBack;
    QSlider * sliderGlobalVolume;
    QSlider * sliderSfxVolume;

    QMap<QString, QLineEdit*> itsKeyLines;
};

#endif // SETTINGVIEW_H
