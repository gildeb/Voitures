## Compilation et chargement du firmware pour le D1 mini (IDE Arduino)
Dans le répertoire Arduino, créer un dossier __D1mini__ et copier dans ce dossier les fichiers _D1mini.ino_, _telecommande_1.h_ et _telecommande_2.h_ . 

Ouvrir _D1mini.ino_ avec l'IDE Arduino et remplacer la constante robot_no par le numéro de robot correspondant (de 1 à 6). Les robots de numéros impairs fonctionnent avec la télécommande 1, les pairs avec la télécommande 2.

Avant de lancer la compilation, selectionner la carte __LOLIN(WEEMOS) D1 mini__ (menu _tools>board_). Il est à priori possible de choisir __Generic ESP8266 Module__ (non testé).
