//#define DEBUG
#include <Arduino.h>

//#define DEBUG

#ifndef MOTEURS
    #define MOTEURS
#endif

#define STOP        1
#define AVANCE      2
#define RECULE      3
#define DROITE      4
#define GAUCHE      5
#define TOUTDROIT   6
#define V1          7
#define V2          8
#define V3          9

#define MAXSPEED    255
#define MINSPEED    70

class Motorisation
{
  public:
     int sens_droite   = D4;
     int sens_gauche   = D3;
     int pwm_droite    = D2;
     int pwm_gauche    = D1;
     int boite_vitesse = 1;
     int sens          = STOP;
     int direction     = TOUTDROIT;
  
     Motorisation();
     void bip(void);
     void action(int commande);
     int boite_de_vitesse(int vitesse);
     void commande_gauche(int vitesse, int sens);
     void commande_droite(int vitesse, int sens);
     void commande_stop();
     void commande_avance();
     void commande_recule();
     void stop(void);
     void avance();
     void recule();
     void gauche();
     void droite();
};
