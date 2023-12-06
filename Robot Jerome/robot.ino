#include "moteurs.h"
#include "ultrason.h"
#include "optique.h"
#include "telecommande.h"
#include "neopix.h"

// modes de fonctionnement
#define MANUEL        10  // neopixel rouge
#define COLLISION     11  // neopixel bleu
#define SUIVI         12  // neopixel jaune
#define EQUILIBRAGE   13

const uint16_t kRecvPin = D5; //ne fonctionne pas sur D0
IRrecv irrecv(kRecvPin);
Telecommande T(&irrecv);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Neopix N(&pixels);
Motorisation M;
Ultrason U(&M);
Optique O(&M);

int Mode = MANUEL;

void setup() {
  Serial.flush();
  Serial.begin(115200);
  // initialisation distance capteur US
  for (int i=0; i<10; i++) {
    int dist = U.lecture();
    delay(100);
  }
  N.neo(ROUGE);  // mode initial = MANUEL
}

void loop() {
  
  String Ircmd = T.read();
  int commande;
  
  if (Ircmd == UP){
    commande = AVANCE;
  } else if (Ircmd == DOWN){
    commande = RECULE;
  } else if (Ircmd == LEFT){
    commande = GAUCHE;
  } else if (Ircmd == RIGHT){
    commande = DROITE;
  } else if (Ircmd == ENTER){
    commande = STOP;
  } else if (Ircmd == BACK){
    if(Mode == MANUEL) {
      Mode = COLLISION;
      N.neo(BLEU);
    }
    else if (Mode == COLLISION){
      Mode = SUIVI;
      N.neo(JAUNE);
    }
    else if (Mode == SUIVI) {
      Mode = MANUEL;
      N.neo(ROUGE);
    }
    M.stop();
    delay(1000);
    commande = 0;
  } else if (Ircmd == PLUS){
    if ( Mode == SUIVI) {
      commande = SPLUS;
    } else {
      M.boite_vitesse = min(M.boite_vitesse + 1 , 3);
    }
  } else if (Ircmd == MOINS){
    if ( Mode == SUIVI) {
      commande = SMOINS;
    } else {
      M.boite_vitesse = max(M.boite_vitesse - 1 , 1);
    }
  } else if (Ircmd == MOUSE){
    Mode = EQUILIBRAGE;
  } else commande = 0;
  
  if (Mode == MANUEL) {
    M.action(commande);
  }
  if (Mode == COLLISION){
    U.action(commande);
  }
  if (Mode == SUIVI){
    O.action(commande);
  }
  if (Mode == EQUILIBRAGE){
    O.equilibrage();
    Mode = SUIVI;
    N.neo(JAUNE);
  }

  delay(100);
}
