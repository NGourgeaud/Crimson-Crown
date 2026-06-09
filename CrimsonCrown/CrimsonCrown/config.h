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

const int playerWidth = 60;
const int playerHeight = 60;

const int BlacksmithAttack1HitboxWidth = 50;
const int BlacksmithAttack1HitboxHeight = 20;

const int BlacksmithAttack2HitboxRadius = 80;

const int BlacksmithAttack1Cooldown = 1;
const int BlacksmithAttack2Cooldown = 5;

const int AssassinAttack1HitboxWidth = 40;
const int AssassinAttack1HitboxHeight = 20;
const float AssassinAttack1Cooldown = 0.3;
const float AssassinAttack2Cooldown = 3.0;

// Hunter Stats
const int HunterAttack1HitboxWidth = 400;
const int HunterAttack1HitboxHeight = 5;

const int HunterAttack2HitboxRadius = 200;

const int HunterAttack1Cooldown = 1;
const int HunterAttack2Cooldown = 5;
>>>>>>> CrimsonCrown/CrimsonCrown/config.h

const int playerRegen = 20;
const float playerIframe = 0.5;

// ==========================================
// --- ENEMY DIMENSIONS AND ATTACKS ---
// ==========================================

// Lancer Stats
const int LancerDefaultMaxHp = 18;
const int LancerDefaultSpeed = 2;
const int LancerDefaultAttackDamage = 3;
const float LancerDefaultAttackCoolDown = 0.5;
const float LancerIframe = 0.2;

const int LancerWidth = 40;
const int LancerHeight = 40;

const int LancerAttackHitboxWidth = 50;
const int LancerAttackHitboxHeight = 20;

// Lord Stats
const int LordDefaultMaxHp = 150;
const int LordDefaultSpeed = 1;
const int LordDefaultAttack1Damage = 10;
const int LordDefaultAttack2Damage = 10;
const float LordDefaultAttack1CoolDown = 5;
const float LordDefaultAttack2CoolDown = 5;
const float LordIframe = 0.2;


const int LordWidth = 50;
const int LordHeight = 50;

const int LordAttack1HitBoxWidth = 100;
const int LordAttack1HitBoxHeight = 100;

const int LordAttack2HitBoxWidth = 40;
const int LordAttack2HitBoxHeight = 30;


enum HeroType {
    None,
    Blacksmith,
    Assassin,
    Hunter
};

enum EnemyType{
    NOTDEF,
    LANCER
};

enum BossType{
    UNDEF,
    LORD
};

enum Team{
    ENEMY,
    PLAYER
};

enum ProjectileType {
    ARROW
};

// --- STATISTIQUES ET DESCRIPTION DU ASSASSIN (HTML STYLISÉ) ---
const QString STATS_ASSASSIN =
    "<p style='font-family: Alagard; font-size: 24px; color: #8A2BE2; margin: 0 0 4px 0;'><b>L'ASSASSIN</b></p>"
    "<p style='font-family: Alagard; font-size: 14px; color: #DDDDDD; margin: 0 0 12px 0;'>"
    "Extrêmement rapide et mortel. Il esquive les combats longs mais ne pardonne pas.</p>"
    "<p style='font-family: Alagard; font-size: 16px; color: white; line-height: 150%; margin: 0;'>"
    "<span style='color: #BBBBBB;'>Vie :</span> <span style='color: #FF3333;'><b>18 HP</b></span><br>"
    "<span style='color: #BBBBBB;'>Vitesse :</span> <span style='color: #33FF33;'><b>12 px/tick</b></span><br>"
    "<span style='color: #BBBBBB;'>Dégâts :</span> <span style='color: #3333FF;'><b>7 HP</b></span>"
    "</p>";


// --- STATISTICS AND DESCRIPTION OF THE BLACKSMITH (STYLISTED HTML) ---
const QString STATS_BLACKSMITH =
    "<p style='font-family: Alagard; font-size: 24px; color: #FFD700; margin: 0 0 4px 0;'><b>LE FORGERON</b></p>"
    "<p style='font-family: Alagard; font-size: 14px; color: #DDDDDD; margin: 0 0 12px 0;'>"
    "Tank lourdement blindé, taillé pour la mêlée. Il encaisse les coups pour le groupe.</p>"
    "<p style='font-family: Alagard; font-size: 16px; color: white; line-height: 150%; margin: 0;'>"
    "<span style='color: #BBBBBB;'>Vie :</span> <span style='color: #FF3333;'><b>40 HP</b></span><br>"
    "<span style='color: #BBBBBB;'>Vitesse :</span> <span style='color: #33FF33;'><b>2 px/tick</b></span><br>"
    "<span style='color: #BBBBBB;'>Dégâts :</span> <span style='color: #3333FF;'><b>13 HP</b></span>"
    "</p>";

const QString STATS_HUNTER =
    "<p style='font-family: Alagard; font-size: 24px; color: #FFD700; margin: 0 0 4px 0;'><b>LE CHASSEUR</b></p>"
    "<p style='font-family: Alagard; font-size: 14px; color: #DDDDDD; margin: 0 0 12px 0;'>"
    "Combattant à distance très mobile mais vulnérable, spécialisé dans le harcèlement continu et le contrôle de zone grâce à ses volées de flèches.</p>"
    "<p style='font-family: Alagard; font-size: 16px; color: white; line-height: 150%; margin: 0;'>"
    "<span style='color: #BBBBBB;'>Vie :</span> <span style='color: #FF3333;'><b>25 HP</b></span><br>"
    "<span style='color: #BBBBBB;'>Vitesse :</span> <span style='color: #33FF33;'><b>10 px/tick</b></span><br>"
    "<span style='color: #BBBBBB;'>Dégâts :</span> <span style='color: #3333FF;'><b>10 HP</b></span>"
    "</p>";


namespace keyBind {
    const Qt::Key keyUp = Qt::Key_Z;
    const Qt::Key keyDown = Qt::Key_S;
    const Qt::Key keyLeft = Qt::Key_Q;
    const Qt::Key keyRight = Qt::Key_D;
    const Qt::Key keyAttaque1 = Qt::Key_K;
    const Qt::Key keyAttaque2 = Qt::Key_L;
}

const float CAMERA_LERP_SPEED = 0.07f;  // Smoothing speed (0.05 = very smooth, 0.2 = very fast)
const float CAMERA_MAX_OFFSET = 100.0f; // Maximum anticipation distance in pixels if running

#endif // CONFIG_H
