// File Name: settingView.h
// Group: A11
// Description: Header file defining the visual interface for the settings screen, including volume controls and keybindings.

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
    ///
    /// \brief Initializes the settings view UI and connections.
    /// \param model Pointer to the setting model containing configuration data.
    /// \param parent Pointer to the parent QWidget.
    ///
    explicit SettingView(SettingModel *model, QWidget *parent = nullptr);

    ///
    /// \brief Constructs and arranges the user interface layout and widgets.
    ///
    void setupUI();

    ///
    /// \brief Updates the UI components (sliders, text inputs) to reflect current values from the model.
    ///
    void updateUIFromModel();

signals:
    ///
    /// \brief Emitted when the back button is clicked to return to the menu.
    ///
    void backRequested();

    ///
    /// \brief Emitted when the user attempts to change a keybinding in the text fields.
    /// \param action The name of the action being rebound.
    /// \param key The new Qt::Key assigned to the action.
    ///
    void keyBindChangedRequested(const QString& action, Qt::Key key);

protected:
    ///
    /// \brief Overrides the paint event to draw the settings screen background.
    /// \param event The paint event triggered by the Qt framework.
    ///
    void paintEvent(QPaintEvent *event) override;

private:
    SettingModel *itsSettingModel;

    QPushButton * boutonBack;
    QSlider * sliderGlobalVolume;
    QSlider * sliderSfxVolume;

    QMap<QString, QLineEdit*> itsKeyLines; };

#endif // SETTINGVIEW_H
