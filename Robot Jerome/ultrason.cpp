#include "ultrason.h"

Ultrason::Ultrason(Motorisation* motor){
    pinMode(this->trigger, OUTPUT); // Sets the trigger as an Output
    pinMode(this->echo, INPUT);     // Sets the echo as an Input
    this->motor = motor;
    for (int i = 0; i < queue; i++) mesures[i] = 0;
}
  
int Ultrason::lecture_brute(){
    long duration;
    digitalWrite(this->trigger, LOW);
    delayMicroseconds(2);
    // Sets the trigger on HIGH state for 10 micro seconds
    digitalWrite(this->trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(this->trigger, LOW);
    // Reads the echo, returns the sound wave travel time in microseconds
    duration = pulseIn(this->echo, HIGH);

    if ((duration < 60000) && (duration > 1)) {
        // Calcule la distance quand la lecture est vraisemblable
        return (duration * SOUND_SPEED/2);
    } else {
        return (0);
    };
}
  
int Ultrason::lecture(){
    int d = this->lecture_brute();
    this->mesures[this->pos] = d;

    int moyenne = 0;

    for (int i = 0; i < queue; i++) moyenne += mesures[i];

    moyenne = int(moyenne/queue);
    this->pos = (this->pos + 1) % queue;

    return (moyenne);
}

void Ultrason::action(int commande){

    if (commande == V1 || commande == V2 || commande == V3) this->motor->action(commande);

    int sens = this->motor->sens;
    int direction = this->motor->direction;
    int obstacle = this->lecture();
    int choix = int(random(DROITE, GAUCHE + 1));

#ifdef DEBUG
    Serial.println("");
    Serial.print("Ultrason action :");
    Serial.print(" obstacle=");
    Serial.print(obstacle);
    Serial.print("cm ; ");
    Serial.print(" commande=");
    Serial.print(commande);
    Serial.print(" ; sens=");
    Serial.print(sens);
    Serial.print(" ; direction=");
    Serial.print(direction);
    Serial.print(" ; choix=");
    Serial.print(choix); 
    Serial.print(" recul=");
    Serial.println(this->recul);
#endif
  
    if (obstacle == 0) return;

    if (commande == 0) {
      if (sens == STOP) return;
      else commande = sens;
    }

    if (commande == STOP) this->motor->stop();
    else {
        if (obstacle < this->seuil2) {
            // on n'a plus la place de tourner
            // on va reculer pendant un certain temps
            this->motor->recule();
            return;
        }
        else if (obstacle < this->seuil1) {
            // on contourne l'obstacle
            if (direction == TOUTDROIT) {
            if (choix == DROITE) this->motor->droite();
            else this->motor->gauche();
            }
            else if (direction == DROITE) this->motor->droite();
            else if (direction == GAUCHE) this->motor->gauche();
        }
        else if (sens != AVANCE || direction != TOUTDROIT) {
            // pas d'obstacle on avance
            this->motor->avance();
        };
    }

    return;
}
