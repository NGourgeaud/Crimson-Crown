// File Name: config.h
// Group: A11
// Description: Global configuration file containing game constants, stats, text strings, and enumerations.

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
const QString LABEL_BACK = "RETOUR AU MENU";

const int playerWidth = 60;
const int playerHeight = 60;

// Blacksmith Stats
const int BlacksmithAttack1HitboxWidth = 60;
const int BlacksmithAttack1HitboxHeight = 30;

const int BlacksmithAttack2HitboxRadius = 80;

const float BlacksmithAttack1Cooldown = 0.7;
const int BlacksmithAttack2Cooldown = 5;

// Assassins Stats
const int AssassinAttack1HitboxWidth = 80;
const int AssassinAttack1HitboxHeight = 30;

const float AssassinAttack1Cooldown = 0.6;

const float AssassinAttack2Duration = 2;
const float AssassinAttack2Cooldown = 7;

// Hunter Stats
const int HunterAttack1HitboxWidth = 40;
const int HunterAttack1HitboxHeight = 5;

const int HunterAttack2HitboxRadius = 200;

const int HunterAttack1Cooldown = 1;
const int HunterAttack2Cooldown = 5;
const float HunterAttack2Duration = 5;
const float HunterAttack2TickRate = 1;

const int playerRegen = 20;
const float playerIframe = 0.5;

// ==========================================
// --- ENEMY DIMENSIONS AND ATTACKS ---
// ==========================================

// Lancer Stats
const int LancerDefaultMaxHp = 25;
const int LancerDefaultSpeed = 2;
const int LancerDefaultAttackDamage = 3;
const float LancerDefaultAttackCoolDown = 0.5;
const float LancerIframe = 0.2;

const int LancerWidth = 40;
const int LancerHeight = 40;

const int LancerAttackHitboxWidth = 50;
const int LancerAttackHitboxHeight = 20;

// Knight Stats
const int KnightDefaultMaxHp = 40;
const int KnightDefaultSpeed = 1;
const int KnightDefaultAttackDamage = 8;
const float KnightDefaultAttackCoolDown = 1;
const float KnightIframe = 0.2;

const int KnightWidth = 40;
const int KnightHeight = 40;

const int KnightAttackHitboxWidth = 30;
const int KnightAttackHitboxHeight = 50;

// Archer Stats
const int ArcherDefaultMaxHp = 18;
const int ArcherDefaultSpeed = 3;
const int ArcherDefaultAttackDamage = 5;
const float ArcherDefaultAttackCoolDown = 1;
const float ArcherIframe = 0.2;

const int ArcherWidth = 40;
const int ArcherHeight = 40;

const int ArcherAttackHitboxWidth = 40;
const int ArcherAttackHitboxHeight = 5;

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

// Clergy Stats
const int ClergyDefaultMaxHp = 150;
const int ClergyDefaultSpeed = 2;
const int ClergyDefaultAttack1Damage = 7;
const int ClergyDefaultAttack2Damage = 7;
const float ClergyDefaultAttack1CoolDown = 3;
const float ClergyDefaultAttack2CoolDown = 6;
const float ClergyIframe = 0.2;

const int ClergyWidth = 50;
const int ClergyHeight = 50;

const int ClergyAttack1HitBoxWidth = 100;
const int ClergyAttack1HitBoxHeight = 100;

const int ClergyAttack2HitBoxWidth = 40;
const int ClergyAttack2HitBoxHeight = 30;

// King Stats
const int KingDefaultMaxHp = 200;
const int KingDefaultSpeed = 1;
const int KingDefaultAttack1Damage = 0;
const int KingDefaultAttack2Damage = 5;
const float KingDefaultAttack1CoolDown = 5;
const float KingDefaultAttack2CoolDown = 5;
const float KingIframe = 0.2;

const int KingWidth = 60;
const int KingHeight = 60;

const int KingAttack2HitBoxWidth = 100;
const int KingAttack2HitBoxHeight = 100;

const int KingAttack2SpikeHitBoxWidth = 70;
const int KingAttack2SpikeHitBoxHeight = 70;

const float KingAttack1Duration = 1.5;
const float KingJumpDuration = 1;
const float KingAttack2Step2Delay = 0.5;

// Drops stats
const int DropSize = 20;

enum HeroType {
    None,
    Blacksmith,
    Assassin,
    Hunter
};

enum EnemyType{
    NOTDEF,
    LANCER,
    KNIGHT,
    ARCHER
};

enum BossType{
    UNDEF,
    LORD,
    KING
};

enum Team{
    ENEMY,
    PLAYER
};

enum ProjectileType {
    ARROW,
    KNIFE,
    RADIUS
};

enum AttackZoneType {
    UNDEFINEDE,
    UNDEFINEDP,
    HAMMER,
    LANCE,
    SWORD
};

enum ItemType {
    HEART,
    GOLD
};

// --- ASSASSIN STATS AND DESCRIPTION ---
const QString STATS_ASSASSIN =
    "<p style='font-family: Alagard; font-size: 24px; color: #FFD700; margin: 0 0 4px 0;'><b>L'ASSASSIN</b></p>"
    "<p style='font-family: Alagard; font-size: 14px; color: #DDDDDD; margin: 0 0 12px 0;'>"
    "Extremely fast and deadly. Evades long fights but shows no mercy.</p>"
    "<p style='font-family: Alagard; font-size: 16px; color: white; line-height: 150%; margin: 0;'>"
    "<span style='color: #BBBBBB;'>Vie :</span> <span style='color: #FF3333;'><b>18 HP</b></span><br>"
    "<span style='color: #BBBBBB;'>Vitesse :</span> <span style='color: #33FF33;'><b>12</b></span><br>"
    "<span style='color: #BBBBBB;'>Dégâts :</span> <span style='color: #3333FF;'><b>6 HP</b></span>"
    "</p>";

// --- BLACKSMITH STATS AND DESCRIPTION ---
const QString STATS_BLACKSMITH =
    "<p style='font-family: Alagard; font-size: 24px; color: #FFD700; margin: 0 0 4px 0;'><b>LE FORGERON</b></p>"
    "<p style='font-family: Alagard; font-size: 14px; color: #DDDDDD; margin: 0 0 12px 0;'>"
    "Heavily armored tank, built for melee. Takes the hits for the group.</p>"
    "<p style='font-family: Alagard; font-size: 16px; color: white; line-height: 150%; margin: 0;'>"
    "<span style='color: #BBBBBB;'>Vie :</span> <span style='color: #FF3333;'><b>40 HP</b></span><br>"
    "<span style='color: #BBBBBB;'>Vitesse :</span> <span style='color: #33FF33;'><b>8</b></span><br>"
    "<span style='color: #BBBBBB;'>Dégâts :</span> <span style='color: #3333FF;'><b>13 HP</b></span>"
    "</p>";

// --- HUNTER STATS AND DESCRIPTION ---
const QString STATS_HUNTER =
    "<p style='font-family: Alagard; font-size: 24px; color: #FFD700; margin: 0 0 4px 0;'><b>LE CHASSEUR</b></p>"
    "<p style='font-family: Alagard; font-size: 14px; color: #DDDDDD; margin: 0 0 12px 0;'>"
    "Highly mobile but vulnerable ranged fighter, specialized in zone control with volleys of arrows.</p>"
    "<p style='font-family: Alagard; font-size: 16px; color: white; line-height: 150%; margin: 0;'>"
    "<span style='color: #BBBBBB;'>Vie :</span> <span style='color: #FF3333;'><b>25 HP</b></span><br>"
    "<span style='color: #BBBBBB;'>Vitesse :</span> <span style='color: #33FF33;'><b>6</b></span><br>"
    "<span style='color: #BBBBBB;'>Dégâts :</span> <span style='color: #3333FF;'><b>10 HP</b></span>"
    "</p>";


namespace keyBind {
inline Qt::Key keyUp = Qt::Key_Z;
inline Qt::Key keyDown = Qt::Key_S;
inline Qt::Key keyLeft = Qt::Key_Q;
inline Qt::Key keyRight = Qt::Key_D;
inline Qt::Key keyAttaque1 = Qt::Key_K;
inline Qt::Key keyAttaque2 = Qt::Key_L;
}

const float CAMERA_LERP_SPEED = 0.07f;  // Smoothing speed (0.05 = very smooth, 0.2 = very fast)
const float CAMERA_MAX_OFFSET = 100.0f; // Maximum anticipation distance in pixels if running

const int BASE_PRICE_HP = 15;
const int BASE_PRICE_DMG = 20;
const int BASE_PRICE_SPEED = 25;
const float PRICE_SCALE = 1.4f;

#endif // CONFIG_H
