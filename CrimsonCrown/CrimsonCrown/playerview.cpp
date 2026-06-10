#include "playerview.h"
#include "assassinmodel.h"
#include "blacksmithmodel.h"
#include "huntermodel.h"
#include <QPainter>

PlayerView::PlayerView(QWidget * parent, PlayerModel * aModel)
    : QWidget{parent}, itsModel(aModel)
{}

void PlayerView::draw(QPainter * painter)
{
    if (!itsModel || itsTexture.isNull()) return;

    // --- 1. GESTION INVISIBILITÉ (Assassin uniquement) ---
    bool isInvisible = false;
    if (AssassinModel* assassin = dynamic_cast<AssassinModel*>(itsModel)) {
        if (assassin->getIsInvisible()) isInvisible = true;
    }
    painter->setOpacity(isInvisible ? 0.4 : 1.0);

    // --- 2. CONFIGURATION DE LA GRILLE UNIVERSELLE (4x4) ---
    int nbRows = 4;
    int nbCols = 4;
    int row = 0;

    // Mappage des directions (Ligne 2 = Droite, Ligne 3 = Gauche)
    switch (itsModel->getItsDirection()) {
    case Down:                                row = 0; break;
    case Up:                                  row = 1; break;
    case Right: case RightUp: case RightDown: row = 2; break;
    case Left: case LeftUp: case LeftDown:    row = 3; break;
    default:                                  row = 0; break;
    }

    int col = 0;
    if (itsModel->isMoving()) {
        col = itsModel->getCurrentFrame() % nbCols;
    }

    // --- 3. CALCUL DU DÉCOUPAGE SANS COUPER LES JAMBES ---
    float cellWidth = static_cast<float>(itsTexture.width()) / nbCols;
    float cellHeight = static_cast<float>(itsTexture.height()) / nbRows;

    // Marges horizontales pour éviter de déborder à gauche/droite
    float marginX = cellWidth * 0.05f;

    // Marges verticales par défaut
    float marginHaut = cellHeight * 0.02f;
    float marginBas = 0.0f; // À 0 pour ne JAMAIS couper les jambes au bas de la case

    // ======================================================================
    // LOGIQUE DE SÉCURITÉ NETTOYÉE PAR DIRECTION
    // ======================================================================
    if (row == 0) {
        marginHaut = cellHeight * 0.04f; // Vue de Face
        marginBas = 0.0f;
    }
    else if (row == 1) {
        marginHaut = cellHeight * 0.11f; // Vue de Dos (on filtre les pieds du dessus)
        marginBas = 0.0f;                // 0% pour garder les pieds intacts !
    }
    else if (row == 2 || row == 3) {
        marginHaut = cellHeight * 0.04f; // Profils
        marginBas = 0.0f;
    }

    // Application du découpage final dans le spritesheet
    float srcX = col * cellWidth + marginX;
    float srcY = row * cellHeight + marginHaut;
    float srcW = cellWidth - (2.0f * marginX);
    float srcH = cellHeight - marginHaut - marginBas;

    QRectF sourceRect(srcX, srcY, srcW, srcH);

    // --- 4. RECTANGLE DE DESTINATION (Taille ajustée à l'écran) ---
    float ratio = srcW / srcH;

    // ======================================================================
    // TAILLE AJUSTÉE : Redescendue à 1.15f pour que le joueur ne soit plus trop grand
    // ======================================================================
    float SCALE_FACTOR = 1.3f;
    float renderHeight = itsModel->getHitBox().height() * SCALE_FACTOR;
    float renderWidth = renderHeight * ratio;

    float centerX = itsModel->getHitBox().center().x();
    float centerY = itsModel->getHitBox().center().y();

    QRectF targetRect(centerX - (renderWidth / 2.0f),
                      centerY - (renderHeight / 2.0f),
                      renderWidth,
                      renderHeight);

    // Désactiver le lissage de Qt pour garder le Pixel Art parfaitement net
    bool oldSmooth = painter->renderHints() & QPainter::SmoothPixmapTransform;
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    // Rendu final
    painter->drawPixmap(targetRect, itsTexture, sourceRect);

    // Restauration du contexte graphique
    painter->setRenderHint(QPainter::SmoothPixmapTransform, oldSmooth);
    painter->setOpacity(1.0);
}

void PlayerView::setItsTexture(HeroType namePlayer)
{
    switch (namePlayer) {
    case HeroType::Blacksmith:
        itsTexture = QPixmap(":/forgeron_sprite.png");
        break;
    case HeroType::Assassin:
        itsTexture = QPixmap(":/assassin_sprite.png");
        break;
    case HeroType::Hunter:
        itsTexture = QPixmap(":/chasseur_sprite.png");
        break;
    default:
        itsTexture = QPixmap();
        break;
    }
}
