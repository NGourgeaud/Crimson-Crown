// File Name: settingcontroller.cpp
// Group: A11
// Description: Controller for the settings menu, bridging the view and the model for volume and keybindings.

#include "settingcontroller.h"

///
/// \brief Initializes the Setting controller and binds the model's volume signals.
/// \param parentWidget Pointer to the container widget (the MainWindow).
/// \param parent Pointer to the parent QObject (the AppController).
///
SettingController::SettingController(QWidget *parentWidget, QObject *parent)
    : QObject(parent),
    itsSettingView(nullptr),
    itsSettingModel(new SettingModel(this)),
    parentWidget(parentWidget)
{
    connect(itsSettingModel, &SettingModel::globalVolumeChanged, this, &SettingController::globalVolumeChanged);
}

///
/// \brief Destructor for the Setting controller.
///
SettingController::~SettingController()
{
}

///
/// \brief Instantiates the SettingView and binds its UI signals.
/// \return Pointer to the created SettingView.
///
QWidget* SettingController::createView()
{
    if (!itsSettingView) {
        itsSettingView = new SettingView(itsSettingModel, parentWidget);

        connect(itsSettingView, &SettingView::backRequested, this, &SettingController::onBackRequested);
        connect(itsSettingView, &SettingView::keyBindChangedRequested, this, &SettingController::onKeyBindChangedRequested);
    }
    return itsSettingView;
}

///
/// \brief Disconnects signals and safely deletes the SettingView from memory.
///
void SettingController::destroyView()
{
    if (itsSettingView == nullptr) return;

    disconnect(itsSettingView);
    itsSettingView->deleteLater();
    itsSettingView = nullptr;
}

///
/// \brief Emits the menuRequested signal to navigate back to the main menu.
///
void SettingController::onBackRequested()
{
    emit menuRequested();
}

///
/// \brief Applies the requested keybind change directly into the model and config.
/// \param action The name of the action being rebound.
/// \param key The new Qt::Key assigned to the action.
///
void SettingController::onKeyBindChangedRequested(const QString &action, Qt::Key key)
{
    itsSettingModel->setKeyBind(action, key);
}
