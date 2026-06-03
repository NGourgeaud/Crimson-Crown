#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

const QString WINDOW_TITLE = "Crimson Crown";
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

const QString MENU_TITLE = "CRIMSON CROWN";
const int MENU_TITLE_FONT_SIZE = 26;

const int BUTTON_HEIGHT = 50;
const int LAYOUT_MARGIN = 40;
const int LAYOUT_SPACING = 15;

const QString LABEL_PLAY = "JOUER";
const QString LABEL_LEADERBOARD = "LEADERBOARD";
const QString LABEL_SETTINGS = "REGLAGES";
const QString LABEL_QUIT = "QUITTER";

const QString LABEL_CHARSELECT_TITLE = "SELECTIONNEZ UN PERSONNAGE";
const QString LABEL_CONTINUE = "CONTINUER";
const QString LABEL_BACK = "RETOUR AU MENU";

const int playerWidth = 0;
const int playerHeight = 0;

const int BlacksmithAttack1HitboxWidth = 50;
const int BlacksmithAttack1HitboxHeight = 20;

const int BlacksmithAttack2HitboxRadius = 40;

const int BlacksmithAttack1Cooldown = 1;
const int BlacksmithAttack2Cooldown = 5;

// / a définir
const int LancerWidth = 0;
const int LancerHeight = 0;

const int LancerAttackHitboxWidth = 0;
const int LancerAttackHitboxHeight = 0;

enum HeroType {
    None,
    Blacksmith
};

// --- STATISTIQUES ET DESCRIPTION DU FORGERON (HTML STYLISÉ) ---
const QString STATS_BLACKSMITH =
    "<p style='font-family: Alagard; font-size: 24px; color: #FFD700; margin: 0 0 4px 0;'><b>LE FORGERON</b></p>"
    "<p style='font-family: Alagard; font-size: 14px; color: #DDDDDD; margin: 0 0 12px 0;'>"
    "Tank lourdement blindé, taillé pour la mêlée. Il encaisse les coups pour le groupe.</p>"
    "<p style='font-family: Alagard; font-size: 16px; color: white; line-height: 150%; margin: 0;'>"
    "<span style='color: #BBBBBB;'>Vie :</span> <span style='color: #FF3333;'><b>40 HP</b></span><br>"
    "<span style='color: #BBBBBB;'>Vitesse :</span> <span style='color: #33FF33;'><b>2 px/tick</b></span><br>"
    "<span style='color: #BBBBBB;'>Dégâts :</span> <span style='color: #3333FF;'><b>13 HP</b></span>"
    "</p>";

#endif // CONFIG_H
