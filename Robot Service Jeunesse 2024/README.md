La version 2024 robot du Servive Jeunesse est guidée par télécommande infra-rouge. Elle embarque une caméra et peut envoyer des photos 
vers un ordinateur fixe dans le but d'effectuer une reconnaissance d'image à l'aide d'un algorithme 
d'intelligence artificielle.

## Liste de materiel:
- __D1 Mini__ :
- __driver moteur L293D__ : 
- __ESP32-cam__
- __diode IR TL1838__ (ou équivalent): https://fr.aliexpress.com/item/4000610270256.html?spm=a2g0o.order_list.order_list_main.114.5f2c5e5bFkj0JY&gatewayAdapt=glo2fra
- __diode neopixel__
- __diode schottky__ : 
- __telecommande IR__ : https://fr.aliexpress.com/item/1005005880586343.html?spm=a2g0o.order_list.order_list_main.5.50145e5bUaWUCF&gatewayAdapt=glo2fra

## Schema electrique
<p align="center">
  <img src="./Robot Jerome.png" width="600">
<p/>

Il faut, bien sûr ajouter l'ESP8266 qui ne figure pas sur le schéma.

La batterie alimente à la fois l'ESP8266 et le driver L293D. Ne pas dépasser 9V : destruction de l'ESP8266 (2 accus Li-ion en série conviennent). __Attention__ à ne pas oublier le jumper (voir schéma).

La rangée de pins Vcc du shield est à 3,3V.
