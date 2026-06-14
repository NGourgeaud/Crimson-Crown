// File Name: settingmodel.h
// Group: A11
// Description: Header file defining the data model for game settings, including audio and keybinds.

#ifndef SETTINGMODEL_H
#define SETTINGMODEL_H

#include <QObject>
#include <QMap>

class SettingModel : public QObject
{
    Q_OBJECT
public:
    ///
    /// \brief Initializes the setting model with default values.
    /// \param parent The parent QObject.
    ///
    explicit SettingModel(QObject *parent = nullptr);

    // Getters
    /// \brief Retrieves the current global volume.
    /// \return The volume level (0-100).
    int getGlobalVolume() const;

    /// \brief Retrieves the current sound effects volume.
    /// \return The SFX volume level (0-100).
    int getSfxVolume() const;

    /// \brief Retrieves the assigned key for an action.
    /// \param action The action name.
    /// \return The assigned Qt::Key.
    Qt::Key getKeyBind(const QString& action) const;

public slots:
    // Setters (which emit signals)
    /// \brief Sets the global volume and emits a signal.
    /// \param volume The new volume level.
    void setGlobalVolume(int volume);

    /// \brief Sets the SFX volume and emits a signal.
    /// \param volume The new volume level.
    void setSfxVolume(int volume);

    /// \brief Updates a keybind and emits a signal.
    /// \param action The action name.
    /// \param key The new key.
    void setKeyBind(const QString& action, Qt::Key key);

signals:
    /// \brief Emitted when global volume is changed.
    void globalVolumeChanged(int volume);

    /// \brief Emitted when SFX volume is changed.
    void sfxVolumeChanged(int volume);

    /// \brief Emitted when a keybind is changed.
    void keyBindChanged(const QString& action, Qt::Key key);

private:
    int itsGlobalVolume;
    int itsSfxVolume;
    QMap<QString, Qt::Key> itsKeyBinds;
};

#endif // SETTINGMODEL_H
