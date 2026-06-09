#include "settingcontroller.h"

SettingController::SettingController(QWidget *parentWidget, QObject *parent)
    : QObject(parent),
    itsSettingView(nullptr),
    itsSettingModel(new SettingModel(this)),
    parentWidget(parentWidget)
{
}

SettingController::~SettingController()
{
}

QWidget* SettingController::createView()
{
    if (!itsSettingView) {
        itsSettingView = new SettingView(itsSettingModel, parentWidget);

        connect(itsSettingView, &SettingView::backRequested, this, &SettingController::onBackRequested);
        connect(itsSettingView, &SettingView::keyBindChangedRequested, this, &SettingController::onKeyBindChangedRequested);
    }
    return itsSettingView;
}

void SettingController::destroyView()
{
    if (itsSettingView == nullptr) return;

    disconnect(itsSettingView);
    itsSettingView->deleteLater();
    itsSettingView = nullptr;
}

void SettingController::onBackRequested()
{
    emit menuRequested();
}

// 👈 Applique directement le changement dans le modèle et dans le config.h
void SettingController::onKeyBindChangedRequested(const QString &action, Qt::Key key)
{
    itsSettingModel->setKeyBind(action, key);
}
