## Compilation et chargement du firmware pour le D1 mini (IDE Arduino)
Dans le répertoire Arduino, créer un répertoire __RSJ 2024__ et copier le fichier RSJ2024.ino. 

Avant de lancer la compilation, selectionner la carte __LOLIN(WEEMOS) D1 mini__. Il est à priori possible de choisir __Generic ESP8266 Module__ (non testé) mais dans ce cas, les pins D0,... D8 ne seront pas définies et il faut les remplacer par les numéros de GPIO correspondants dans les fichiers .ino, et .h (D0=16, D5=14, ...).
