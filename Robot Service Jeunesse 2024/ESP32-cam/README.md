Deux versions possibles du driver de l'ESP32-cam:

- une version C (repertoire Arduino) : les images de la caméra sont transmises au PC via des requetes HTTP. La diode neopixel n'est pas utilisée.
- une version micropython (répertoire micropython) : les images sont transférées sous la forme de trames udp. La diode neopixel est utilisée pour
  visualiser sur le robot la réponse de l'ocr (rouge=aucun chiffre identifié, bleu=chiffre identifié avec proba<0.8, vert=chiffre identifié avec
  proba>0.8).
