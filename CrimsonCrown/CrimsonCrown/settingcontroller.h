// File Name: settingcontroller.h
// Group: A11
// Description: Header file defining the controller for the settings screen.

#ifndef SETTINGCONTROLLER_H
#define SETTINGCONTROLLER_H

#include <QObject>
#include "settingmodel.h"
#include "settingView.h"

class SettingController : public QObject
{
    Q_OBJECT
public:
    ///
    /// \brief Initializes the settings controller.
    /// \param parentWidget The parent QWidget.
    /// \param parent The parent QObject.
    ///
    explicit SettingController(QWidget *parentWidget = nullptr, QObject *parent = nullptr);

    ///
    /// \brief Destructor handling cleanup.
    ///
    ~SettingController();

    ///
    /// \brief Creates the settings view and binds signals.
    /// \return Pointer to the generated view.
    ///
    QWidget* createView();

    ///
    /// \brief Destroys the view and frees memory.
    ///
    void destroyView();

signals:
    /// \brief Emitted to request a return to the main menu.
    void menuRequested();

    /// \brief Emitted when the global volume changes.
    /// \param volume The new volume.
    void globalVolumeChanged(int volume);

private slots:
    /// \brief Handles the back request.
    void onBackRequested();

    ///
    /// \brief Processes a requested keybind change.
    /// \param action The action to rebind.
    /// \param key The new Qt::Key to assign.
    ///
    void onKeyBindChangedRequested(const QString &action, Qt::Key key);

private:
    SettingView *itsSettingView;
    SettingModel *itsSettingModel;
    QWidget *parentWidget;
};

#endif // SETTINGCONTROLLER_H
