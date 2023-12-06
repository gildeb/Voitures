#include <Arduino.h>

#ifndef MOTEURS
    #define MOTEURS
    #include "moteurs.h"
#endif

//#define DEBUG

#define MAXSENSIB   50  // seuil max de sensibilité
#define MINSENSIB   5   // seuil min de sensibilité
#define SPLUS       16
#define SMOINS      17

class Optique
{
private:
    Motorisation* motor = NULL;
    int capteur = A0;         // lecture

public:
    int sensibilite = (MAXSENSIB + MINSENSIB)/2;     // seuil de sensibilite droite/gauche
    int equilibrage_fait  = 0;
    long valeur_equilibre = 0;  // par défaut. En faisant l'equilibrage on peut ajuster cette valeur

    Optique(Motorisation* motor);
    long lecture();
    void equilibrage();
    long delta();
    void set_sensibilite(int sensibilite);
    void action(int commande);
};
