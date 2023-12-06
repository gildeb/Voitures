#include "optique.h"

Optique::Optique(Motorisation* motor){
    this->motor = motor;
}

long Optique::lecture(){
    return (analogRead(this->capteur));
}

void Optique::equilibrage(){

    this->motor->stop();
    // positionner une feuille blanche très près du capteur optique
     long valeur_brute = this->lecture();
     this->valeur_equilibre = 512 - valeur_brute;
     this->equilibrage_fait = 1;
     
#ifdef DEBUG
     Serial.println("");
     Serial.println("Optique: Equilibrage");
     Serial.print("  valeur_brute:");
     Serial.println(valeur_brute);
     Serial.print("  this->valeur_equilibre:");
     Serial.println(this->valeur_equilibre);
#endif
}

long Optique::delta(){
    // mesure la différence entre le capteur de droite et le capteur de gauche
    // delta > 0 => lumière à droite
    // delta < 0 => lumière à gauche

    long valeur = 512 - this->lecture();
    
    return (valeur - this->valeur_equilibre);
}

void  Optique::set_sensibilite(int sensibilite) {
    this->sensibilite = max(min(sensibilite, MAXSENSIB), MINSENSIB);
#ifdef DEBUG
    Serial.println("");
    Serial.print("  this->sensibilite:");
    Serial.println(this->sensibilite);
#endif
    return;
}

void Optique::action(int commande){
    
    int sens = this->motor->sens;
    
    if (commande == V1 || commande == V2 || commande == V3) {
       this->motor->action(commande);
       commande = sens;
    } else if (commande == SPLUS) {
       this->set_sensibilite(this->sensibilite + 5);
       commande = sens;
    } else if (commande == SMOINS) {
       this->set_sensibilite(this->sensibilite - 5);
       commande = sens;
    } else if (commande == 0){
      if (sens == STOP) return;
      else commande = sens;
    }
    
    int direction = this->motor->direction;       
    long valeur_brute = this->lecture();
    long valeur = 512 - valeur_brute;
    int delta = this->delta();

#ifdef DEBUG
    Serial.println("");
    Serial.print("Optique action> commande=");
    Serial.print(commande);
    Serial.print(" delta=");
    Serial.print(delta);
//    Serial.print(" lecture brute=");
//    Serial.print(valeur_brute);
//    Serial.print(" lecture=");
//    Serial.print(valeur);
//    Serial.print(" valeur_equilibre=");
//    Serial.print(this->valeur_equilibre);
    Serial.print(" sensibilite=");
    Serial.print(this->sensibilite);
#endif
    
    if (commande == STOP) this->motor->stop();
    else {
        if (abs(delta) < this->sensibilite)  { // pas de différence notable
            if (this->motor->sens != AVANCE) this->motor->avance();
#ifdef DEBUG
             Serial.print(" tout droit");
#endif  
        } else if (delta > 0 ) {       // lumière à droite => on tourne à droite
            if (sens == AVANCE && direction == DROITE) return;
#ifdef DEBUG
             Serial.print(" tourne à droite");
#endif
            this->motor->droite();
        } else if (delta < 0) { // lumière à gauche => on tourne à gauche
            if (sens == AVANCE && direction == GAUCHE ) return;
#ifdef DEBUG
             Serial.print(" tourne à gauche");
#endif
            this->motor->gauche();
        }
    }
    return;
}
