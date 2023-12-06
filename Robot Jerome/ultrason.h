//#define DEBUG
#include <Arduino.h>

#ifndef MOTEURS
    #define MOTEURS
    #include "moteurs.h"
#endif

class Ultrason{

#define SOUND_SPEED 0.034  //sound speed in cm/uS
#define queue 5            // nb de  mesures pour le calcul de la moyenne

private:
    const int trigger = D7;
    const int echo    = D6;
    const int seuil1  = 60;  // si > seuil1 on avance, sinon on tourne à droite
    const int seuil2  = 20;  // si < seuil2 on stoppe 
    Motorisation* motor = NULL;
    int mesures[queue];
    int pos = 0;             // indice tournant dans le tampon
    int recul = 0;

public:
  Ultrason(Motorisation* motor);   // constructeur
  int lecture_brute();
  int lecture();
  void action(int commande);
};
