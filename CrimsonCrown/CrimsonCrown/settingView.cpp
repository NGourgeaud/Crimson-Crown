#include "settingView.h"
#include "settingmodel.h"
#include "config.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QKeySequence>

SettingView::SettingView(SettingModel *model, QWidget *parent)
    : QWidget(parent), itsSettingModel(model)
{
    setupUI();
    updateUIFromModel();

    connect(itsSettingModel, &SettingModel::globalVolumeChanged, sliderGlobalVolume, &QSlider::setValue);
    connect(itsSettingModel, &SettingModel::sfxVolumeChanged, sliderSfxVolume, &QSlider::setValue);
    connect(itsSettingModel, &SettingModel::keyBindChanged, this, &SettingView::updateUIFromModel);
}

void SettingView::setupUI()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(LAYOUT_MARGIN * 1.5, LAYOUT_MARGIN * 1.5, LAYOUT_MARGIN * 1.5, LAYOUT_MARGIN * 1.5);
    mainLayout->setSpacing(50);

    QString labelStyle = "color: white; font-family: 'Alagard'; font-size: 18px; background: transparent;";

    // Style arcade pour nos zones de texte
    QString lineEditStyle =
        "QLineEdit {"
        "   background-color: #111111;"
        "   color: white;"
        "   border: 2px solid #555555;"
        "   font-family: 'Alagard';"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border-radius: 3px;"
        "   alignment: center;"
        "}"
        "QLineEdit:focus { border-color: #E30000; }";

    QString arcadeButtonStyle =
        "QPushButton { background-color: #111111; color: white; border: 2px solid #555555; font-family: 'Alagard'; font-size: 15px; font-weight: bold; border-radius: 3px; }"
        "QPushButton:hover { background-color: #E30000; border-color: white; }";

    // --- PANNEAU GAUCHE ---
    QVBoxLayout *leftPanel = new QVBoxLayout();
    leftPanel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QLabel *titleLabel = new QLabel("REGLAGES", this);
    QFont titleFont("Alagard");
    titleFont.setPointSize(MENU_TITLE_FONT_SIZE);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: white; background: transparent; margin-bottom: 20px;");
    leftPanel->addWidget(titleLabel);

    QGridLayout *keyLayout = new QGridLayout();
    keyLayout->setSpacing(15);
    QStringList actions = {"Avancer", "Reculer", "Gauche", "Droite", "Attaque", "Attaque Spé."};

    for (int i = 0; i < actions.size(); ++i) {
        QLabel *lbl = new QLabel(actions[i], this);
        lbl->setStyleSheet(labelStyle);

        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setFixedSize(50, BUTTON_HEIGHT - 15);
        lineEdit->setStyleSheet(lineEditStyle);
        lineEdit->setMaxLength(1);
        lineEdit->setAlignment(Qt::AlignCenter);

        itsKeyLines[actions[i]] = lineEdit;

        // Détection automatique dès que la lettre change
        connect(lineEdit, &QLineEdit::textEdited, this, [this, action = actions[i]](const QString &text) {
            if (!text.isEmpty()) {
                QString upperText = text.toUpper();

                // On force l'affichage en majuscule dans la zone de texte
                itsKeyLines[action]->blockSignals(true);
                itsKeyLines[action]->setText(upperText);
                itsKeyLines[action]->blockSignals(false);

                // Conversion de la lettre en Qt::Key (la valeur unicode correspond aux enums Qt::Key_A à Z)
                Qt::Key pressedKey = static_cast<Qt::Key>(upperText.at(0).unicode());
                emit keyBindChangedRequested(action, pressedKey);
            }
        });

        keyLayout->addWidget(lbl, i, 0);
        keyLayout->addWidget(lineEdit, i, 1);
    }

    QHBoxLayout *controlsLayout = new QHBoxLayout();
    controlsLayout->addLayout(keyLayout);
    controlsLayout->addSpacing(60);

    // Sliders
    QVBoxLayout *slidersLayout = new QVBoxLayout();
    slidersLayout->setSpacing(10);

    QLabel *lblGlobal = new QLabel("Volume Global", this);
    lblGlobal->setStyleSheet(labelStyle);
    sliderGlobalVolume = new QSlider(Qt::Horizontal, this);
    sliderGlobalVolume->setRange(0, 100);
    sliderGlobalVolume->setFixedWidth(200);
    sliderGlobalVolume->setStyleSheet("QSlider::groove:horizontal { border: 1px solid #555; height: 6px; background: #222; } QSlider::handle:horizontal { background: #E30000; width: 12px; margin: -3px 0; border-radius: 6px; }");

    QLabel *lblSfx = new QLabel("Effet Sonore", this);
    lblSfx->setStyleSheet(labelStyle);
    sliderSfxVolume = new QSlider(Qt::Horizontal, this);
    sliderSfxVolume->setRange(0, 100);
    sliderSfxVolume->setFixedWidth(200);
    sliderSfxVolume->setStyleSheet(sliderGlobalVolume->styleSheet());

    connect(sliderGlobalVolume, &QSlider::valueChanged, itsSettingModel, &SettingModel::setGlobalVolume);
    connect(sliderSfxVolume, &QSlider::valueChanged, itsSettingModel, &SettingModel::setSfxVolume);

    slidersLayout->addWidget(lblGlobal);
    slidersLayout->addWidget(sliderGlobalVolume);
    slidersLayout->addSpacing(15);
    slidersLayout->addWidget(lblSfx);
    slidersLayout->addWidget(sliderSfxVolume);
    slidersLayout->addStretch();

    controlsLayout->addLayout(slidersLayout);
    leftPanel->addLayout(controlsLayout);
    leftPanel->addSpacing(30);

    boutonBack = new QPushButton("RETOUR", this);
    boutonBack->setFixedSize(220, BUTTON_HEIGHT);
    boutonBack->setStyleSheet(arcadeButtonStyle);
    connect(boutonBack, &QPushButton::clicked, this, &SettingView::backRequested);
    leftPanel->addWidget(boutonBack);

    // --- PANNEAU DROITE ---
    QVBoxLayout *rightPanel = new QVBoxLayout();
    rightPanel->setAlignment(Qt::AlignCenter);

    mainLayout->addLayout(leftPanel, 1);
    mainLayout->addLayout(rightPanel, 1);
    this->setStyleSheet("background: transparent;");
}

void SettingView::updateUIFromModel()
{
    sliderGlobalVolume->setValue(itsSettingModel->getGlobalVolume());
    sliderSfxVolume->setValue(itsSettingModel->getSfxVolume());

    // Met à jour la lettre affichée dans chaque case de texte
    for (auto it = itsKeyLines.begin(); it != itsKeyLines.end(); ++it) {
        Qt::Key key = itsSettingModel->getKeyBind(it.key());
        QString keyString = QKeySequence(key).toString().toUpper();

        it.value()->blockSignals(true);
        it.value()->setText(keyString);
        it.value()->blockSignals(false);
    }
}

void SettingView::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap background(":/fondjeu.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), background);
}
