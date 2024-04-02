Version micropython du driver de l'ESP32-cam

Pour charger le firmware micropython dans l'ESP32-cam, voir :

https://github.com/lemariva/micropython-camera-driver

Charger ensuite les fichiers _RSJ2024_imager_server.py_ et _main.py_ à la racine du système de fichier.

Le serveur d'images est lancé automatiquement au redémarrage.

Sur le PC:
- se connecter au point d'accès wifi 'ESP32-cam-x' (x=robot_nb entre 1 et 6)
- lancer _Mastermind_Micropython.py_
