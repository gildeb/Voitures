La version 2024 robot du Servive Jeunesse est guidée par télécommande infra-rouge. Elle embarque une caméra et peut envoyer des photos 
vers un ordinateur fixe dans le but d'effectuer une reconnaissance d'image à l'aide d'un algorithme 
d'intelligence artificielle.

## Liste de materiel:
- __D1 Mini (ESP8266)__ : (aliexpress)
- __driver moteur L293D__ : https://www.gotronic.fr/art-l293d-14072.htm
- __ESP32-cam__ :  (aliexpress)
- __diode IR TL1838__ (ou équivalent): https://fr.aliexpress.com/item/4000610270256.html?spm=a2g0o.order_list.order_list_main.114.5f2c5e5bFkj0JY&gatewayAdapt=glo2fra
- __diode neopixel__ : https://www.gotronic.fr/art-5-leds-rgb-5mm-neopixel-ada1938-22883.htm
- __diode schottky__ : https://www.gotronic.fr/art-1n5819-104.htm
- __telecommande IR H713__ : (aliexpress)

## Schema electrique
<p align="center">
  <img src="./Schema electrique.png" width="600">
<p/>

Le circuit électrique utilise deux microcontroleurs : 
- un ESP8266 (D1 Mini) pour la réception du signal infre-rougen et la commande des moteurs,
- un ESP32-cam pour la capture des images.

Ils sont alimentés en 5V par la même source : 4 piles 1,5V ou 4 accus NiMh de 1,2V.

<p align="center">
  <img src="./RSJ_implantation.png" width="400">
<p/>

## Compilation et chargement du firmware (IDE Arduino)
Dans le répertoire Arduino, créer un répertoire __RSJ 2024__ et copier tous le fichier RSJ2024.ino. 

Avant de lancer la compilation, selectionner la carte __LOLIN(WEEMOS) D1 mini__. Il est à priori possible de choisir __Generic ESP8266 Module__ (non testé) mais dans ce cas, les pins D0,... D8 ne seront pas définies et il faut les remplacer par les numéros de GPIO correspondants dans les fichiers .ino, et .h (D0=16, D5=14, ...).
