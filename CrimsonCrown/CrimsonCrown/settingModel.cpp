#include "settingmodel.h"
#include "config.h"

SettingModel::SettingModel(QObject *parent) : QObject(parent), itsGlobalVolume(50), itsSfxVolume(50)
{
    itsKeyBinds["Avancer"] = keyBind::keyUp;
    itsKeyBinds["Reculer"] = keyBind::keyDown;
    itsKeyBinds["Gauche"] = keyBind::keyLeft;
    itsKeyBinds["Droite"] = keyBind::keyRight;
    itsKeyBinds["Attaque"] = keyBind::keyAttaque1;
    itsKeyBinds["Attaque Spé."] = keyBind::keyAttaque2;
}

int SettingModel::getGlobalVolume() const {
    return itsGlobalVolume;
}

int SettingModel::getSfxVolume() const{
    return itsSfxVolume;
}

Qt::Key SettingModel::getKeyBind(const QString& action) const {
    return itsKeyBinds.value(action, Qt::Key_unknown);
}

void SettingModel::setGlobalVolume(int volume) {
    if (itsGlobalVolume != volume) {
        itsGlobalVolume = volume;
        emit globalVolumeChanged(itsGlobalVolume);
    }
}

void SettingModel::setSfxVolume(int volume) {
    if (itsSfxVolume != volume) {
        itsSfxVolume = volume;
        emit sfxVolumeChanged(itsSfxVolume);
    }
}

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
