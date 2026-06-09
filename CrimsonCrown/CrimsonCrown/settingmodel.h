#ifndef SETTINGMODEL_H
#define SETTINGMODEL_H

#include <QObject>
#include <QMap>

class SettingModel : public QObject
{
    Q_OBJECT
public:
    explicit SettingModel(QObject *parent = nullptr);

    // Getters
    int getGlobalVolume() const;
    int getSfxVolume() const;
    Qt::Key getKeyBind(const QString& action) const;

public slots:
    // Setters (qui émettront les signaux)
    void setGlobalVolume(int volume);
    void setSfxVolume(int volume);
    void setKeyBind(const QString& action, Qt::Key key);

signals:
    void globalVolumeChanged(int volume);
    void sfxVolumeChanged(int volume);
    void keyBindChanged(const QString& action, Qt::Key key);

private:
    int itsGlobalVolume;
    int itsSfxVolume;
    QMap<QString, Qt::Key> itsKeyBinds;
};

#endif // SETTINGMODEL_H
