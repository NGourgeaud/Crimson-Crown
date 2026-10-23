# 👑 Crimson Crown

**Crimson Crown** est un jeu d'action en 2D développé en C++ avec le framework Qt. Le joueur incarne un héros (Assassin, Forgeron, etc.) devant survivre face à des vagues d'ennemis possédant leur propre intelligence artificielle, tout en naviguant dans des salles générées avec un système physique de collisions complet.

Ce projet a été réalisé dans le cadre de la première année de BUT Informatique à l'IUT de La Rochelle (Année 2025-2026).

---

## 🚀 Fonctionnalités Principales

* **Architecture MVC :** Séparation stricte entre la logique métier (`GameModel`, `PlayerModel`, `EnemyModel`), les vues (`MenuView`, `CharSelectView`) et les contrôleurs.
* **Moteur Physique & Collisions :** Système de "Smart Sliding" permettant aux entités de glisser le long des obstacles sans rester bloquées, et gestion précise des hitboxes (joueur, ennemis, boss, murs).
* **Intelligence Artificielle :** Comportements distincts selon le type d'ennemi :
    * *Lancer / Knight :* Poursuite agressive au corps-à-corps.
    * *Archer :* Maintien à distance, fuite si le joueur s'approche trop, et attaques à distance.
* **Système de Sauvegarde :** Classement local des meilleurs scores (`LeaderboardModel`) sauvegardé dynamiquement à la fin de chaque partie.

---

## 🛠️ Prérequis techniques

Pour compiler et exécuter ce projet localement, vous aurez besoin de :
* **Qt Creator** (avec Qt 6.x)
* **Compilateur C++** supportant C++17 ou supérieur (MinGW sous Windows, GCC sous Linux, ou Clang sous macOS).
**OU**
**Télécharger l'exécutables disponible sur teams**
---

## 🕹️ Installation et Compilation

1.  **Cloner le dépôt :**
    ```bash
    git clone https://github.com/NGourgeaud/Crimson-Crown.git
    ```

2.  **Via l'interface Qt Creator :**
    * Ouvrez le fichier `CrimsonCrown.pro`.
    * Configurez votre kit.
    * Cliquez sur le marteau pour compiler, puis sur le bouton "Play" pour lancer le jeu.

Note: 
   Le jeu est compilé et prêt a être lançé dans la branche main dans le fichier `CrimsonCrown-exe` ou l'exécutable est présent.

---

## 👥 Auteurs

**Groupe A11**
**Arthur Cesbron, Antoine Morin, Quentin Pelissier, Nicolas Gourgeaud** 
