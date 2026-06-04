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

const int playerWidth = 100;
const int playerHeight = 100;

const int BlacksmithAttack1HitboxWidth = 50;
const int BlacksmithAttack1HitboxHeight = 20;

const int BlacksmithAttack2HitboxRadius = 40;

const int BlacksmithAttack1Cooldown = 1;
const int BlacksmithAttack2Cooldown = 5;

// ===================
// Ennemies Stats
// ===================

// Lancer Stats
const int LancerDefaultMaxHp = 18;
const int LancerDefaultSpeed = 2;
const int LancerDefaultAttackDamage = 6;
const float LancerDefaultAttackCoolDown = 0.5;

const int LancerWidth = 80;
const int LancerHeight = 80;

const int LancerAttackHitboxWidth = 0;
const int LancerAttackHitboxHeight = 0;

// Lord Stats
const int LordDefaultMaxHp = 150;
const int LordDefaultSpeed = 1;
const int LordDefaultAttack1Damage = 10;
const int LordDefaultAttack2Damage = 10;
const float LordDefaultAttack1CoolDown = 5;
const float LordDefaultAttack2CoolDown = 5;


const int LordWidth = 125;
const int LordHeight = 125;

const int LordAttack1HitBoxWidth = 100;
const int LordAttack1HitBoxHeight = 100;

// a changer en fonction de la taille du lord
const int LordAttack2HitBoxWidth = 40;
const int LordAttack2HitBoxHeight = 30;

enum HeroType {
    None,
    Blacksmith
};

enum EnemyType{
    NOTDEF,
    LANCER
};

enum BossType{
    UNDEF,
    LORD
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



namespace keyBind {
    const Qt::Key keyUp = Qt::Key_Z;
    const Qt::Key keyDown = Qt::Key_S;
    const Qt::Key keyLeft = Qt::Key_Q;
    const Qt::Key keyRight = Qt::Key_D;
    const Qt::Key keyAttaque1 = Qt::Key_K;
    const Qt::Key keyAttaque2 = Qt::Key_L;
}

const float CAMERA_LERP_SPEED = 0.07f;  // Vitesse de lissage (0.05 = très fluide, 0.2 = très rapide)
const float CAMERA_MAX_OFFSET = 100.0f; // Distance max d'anticipation en pixels s'il court

#endif // CONFIG_H
