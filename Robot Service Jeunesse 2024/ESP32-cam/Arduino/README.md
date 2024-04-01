Version Arduino du driver de l'ESP32-cam

## Compilation du firmware pour l'ESP32-cam (IDE Arduino)

Dans le répertoire Arduino, créer un dossier __ESP32-cam__ et copier dans ce dossier les 4 fichiers ci-dessous (.ino, .h et .cpp), puis ouvrir le fichier _ESP32-cam.ino_ dans l'IDE Arduino.

Avant de lancer la compilation, sélectionner la carte __AI Thinker ESP32-CAM__ (menu _tools>board_).

Pour le téléchargement du code, il faut utiliser un convertisseur USB-série :

<p align="center">
  <img src="./branchement convertisseur.png" width="500">
<p/>

__ATTENTION__ : ne pas oublier de connecter la GPIO0 avec la masse pendant le téléchargement. Supprimer cette connexion une fois le téléchargement terminé.
