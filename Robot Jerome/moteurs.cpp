#include "moteurs.h"

Motorisation::Motorisation(){
#ifdef DEBUG
         Serial.println("Motorisation> init");
#endif

         pinMode(this->pwm_gauche,  OUTPUT);
         pinMode(this->pwm_droite,  OUTPUT);
         pinMode(this->sens_gauche, OUTPUT);
         pinMode(this->sens_droite, OUTPUT);

         analogWriteFreq(100);
         this->stop();
}

void Motorisation::bip(void)  { // test moteur
#ifdef DEBUG
          Serial.println("Motorisation> bip");
#endif
          this->avance();
          delay(100);
          this->stop();
          delay(400);
}

void Motorisation::action(int commande){
         if (commande == AVANCE) this->avance();
         else if (commande == RECULE) this->recule();
         else if (commande == DROITE) this->droite();
         else if (commande == GAUCHE) this->gauche();
         else if (commande == STOP) this->stop();
         else if (commande == V1) this->boite_vitesse = 1;
         else if (commande == V2) this->boite_vitesse = 2;
         else if (commande == V3) this->boite_vitesse = 3;
}

int Motorisation::boite_de_vitesse(int vitesse){
         if (vitesse == 0) return (0);
         return (map(vitesse, 1, 3, MINSPEED, MAXSPEED));
}

void Motorisation::commande_gauche(int vitesse, int sens) {
         int v = this->boite_de_vitesse(vitesse);
#ifdef DEBUG
         Serial.print("VG=");
         Serial.println(v);
#endif
         digitalWrite(this->sens_gauche, sens);
         analogWrite(this->pwm_gauche, MAXSPEED);
         delay(10);
         analogWrite(this->pwm_gauche, v);
}

void Motorisation::commande_droite(int vitesse, int sens) {
         int v = this->boite_de_vitesse(vitesse);
#ifdef DEBUG
         Serial.print("VD=");
         Serial.println(v);
#endif
         digitalWrite(this->sens_droite, sens);
         analogWrite(this->pwm_droite, MAXSPEED);
         delay(10);
         analogWrite(this->pwm_droite, v);
}

void Motorisation::commande_stop() {
          this->commande_gauche(0, LOW);
          this->commande_droite(0, LOW);
}

void Motorisation::commande_avance() {
          this->commande_gauche(this->boite_vitesse, HIGH);
          this->commande_droite(this->boite_vitesse, HIGH);
}

void Motorisation::commande_recule() {
          this->commande_gauche(this->boite_vitesse, LOW);
          this->commande_droite(this->boite_vitesse, LOW);
}

void Motorisation::stop(void)  {
#ifdef DEBUG
          Serial.println("Motorisation> stop");
#endif
          this->sens = STOP;
          this->direction = TOUTDROIT;
          this->commande_stop();
}

void Motorisation::avance()  {
#ifdef DEBUG
          Serial.println("Motorisation> avance");
#endif
          this->sens = AVANCE;
          this->direction = TOUTDROIT;
          commande_avance();
}

void Motorisation::recule()  {
#ifdef DEBUG
          Serial.println("Motorisation> recule");
#endif
          this->sens = RECULE;
          commande_recule();
}

void Motorisation::gauche()  {
#ifdef DEBUG
          Serial.println("Motorisation> gauche dir=" + String(this->sens));
#endif
          this->sens = AVANCE;
          this->direction = GAUCHE;
          this->commande_gauche(0, LOW);
          this->commande_droite(1, HIGH);
}

void Motorisation::droite()  {
#ifdef DEBUG
          Serial.println("Motorisation> droite dir=" + String(this->sens));
#endif
          this->sens = AVANCE;
          this->direction = DROITE;
          this->commande_gauche(1, HIGH);
          this->commande_droite(0, LOW);
}
