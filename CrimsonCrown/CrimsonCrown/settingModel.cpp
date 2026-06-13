// File Name: settingModel.cpp
// Group: A11
// Description: Data model handling the application's configuration, including audio volumes and custom keybindings.

#include "settingmodel.h"
#include "config.h"

///
/// \brief Constructs the setting model with default volume and keybind values.
/// \param parent Pointer to the parent QObject.
///
SettingModel::SettingModel(QObject *parent) : QObject(parent), itsGlobalVolume(50), itsSfxVolume(50)
{
    itsKeyBinds["Avancer"] = keyBind::keyUp;
    itsKeyBinds["Reculer"] = keyBind::keyDown;
    itsKeyBinds["Gauche"] = keyBind::keyLeft;
    itsKeyBinds["Droite"] = keyBind::keyRight;
    itsKeyBinds["Attaque"] = keyBind::keyAttaque1;
    itsKeyBinds["Attaque Spé."] = keyBind::keyAttaque2;
}

///
/// \brief Retrieves the current global volume.
/// \return An integer representing the volume level (0-100).
///
int SettingModel::getGlobalVolume() const {
    return itsGlobalVolume;
}

///
/// \brief Retrieves the current sound effects volume.
/// \return An integer representing the SFX volume level (0-100).
///
int SettingModel::getSfxVolume() const{
    return itsSfxVolume;
}

///
/// \brief Retrieves the assigned key for a specific action.
/// \param action The name of the action.
/// \return The Qt::Key bound to the given action.
///
Qt::Key SettingModel::getKeyBind(const QString& action) const {
    return itsKeyBinds.value(action, Qt::Key_unknown);
}

///
/// \brief Sets a new global volume and emits a change signal if modified.
/// \param volume The new volume level (0-100).
///
void SettingModel::setGlobalVolume(int volume) {
    if (itsGlobalVolume != volume) {
        itsGlobalVolume = volume;
        emit globalVolumeChanged(itsGlobalVolume);
    }
}

///
/// \brief Sets a new SFX volume and emits a change signal if modified.
/// \param volume The new volume level (0-100).
///
void SettingModel::setSfxVolume(int volume) {
    if (itsSfxVolume != volume) {
        itsSfxVolume = volume;
        emit sfxVolumeChanged(itsSfxVolume);
    }
}

///
/// \brief Updates the keybind for a specific action and updates the global config variables.
/// \param action The name of the action to rebind.
/// \param key The new Qt::Key to assign.
///
void SettingModel::setKeyBind(const QString& action, Qt::Key key) {
    if (itsKeyBinds.value(action) != key) {
        itsKeyBinds[action] = key;

        if (action == "Avancer") {
            keyBind::keyUp = key;
        } else if (action == "Reculer") {
            keyBind::keyDown = key;
        } else if (action == "Gauche") {
            keyBind::keyLeft = key;
        } else if (action == "Droite") {
            keyBind::keyRight = key;
        } else if (action == "Attaque") {
            keyBind::keyAttaque1 = key;
        } else if (action == "Attaque Spé.") {
            keyBind::keyAttaque2 = key;
        }

        emit keyBindChanged(action, key);
    }
}
