#ifndef SETTINGCONTROLLER_H
#define SETTINGCONTROLLER_H

#include <QObject>
#include "settingmodel.h"
#include "settingView.h" // Corrigé avec la majuscule !

class SettingController : public QObject
{
    Q_OBJECT
public:
    explicit SettingController(QWidget *parentWidget = nullptr, QObject *parent = nullptr);
    ~SettingController();

    QWidget* createView();
    void destroyView();

signals:
    void menuRequested();

private slots:
    void onBackRequested();
    void onKeyBindChangedRequested(const QString &action, Qt::Key key);

private:
    SettingView *itsSettingView;
    SettingModel *itsSettingModel;
    QWidget *parentWidget;
};

#endif // SETTINGCONTROLLER_H
