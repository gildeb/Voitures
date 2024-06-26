// Equivalences entre la notation des pins du ESP2866 Nodemcu
//
//  référence: pinout description
// https://github.com/anumby-source/RobotServiceJeunesse2024/tree/main/ESP8266%20Mini%2CNodeMCU#pins-esp8266-nodemcu-v3
//
//   les broches D1 .. D8 .. D12 correspondent aux GPIO<n> qui peuvent être configurées au choix en INPUT ou OUTPUT

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#define robot_nb 1 // numero robot

#if (robot_nb % 2 == 1)
   #include "telecommande_1.h" 
#else
   #include "telecommande_2.h"
#endif

const int receiverPin = 13; // Broche du récepteur IR
IRrecv irrecv(receiverPin);
decode_results results;

//définitions des paramètres moteur et commande de l'ESP32-cam
const int EnaD   = 5;    // enable du moteur Gauche=GPIO5
const int EnaG   = 14;   // enable du moteur Droit
const int fwdD   = 4;    // forward du moteur Gauche
const int bwdD   = 0;    // backward du moteur Gauche
const int fwdG   = 12;   // forward du moteur Droit
const int bwdG   = 15;   // backward du moteur Droit
const int cam    = 2;    // activation de la caméra
const int ledPin = 2;  // board led

int DCG; //Duty Cycle du moteur Gauche
int DCD; //Duty Cycle du moteur Droit
int DC = 800;  // Duty cycle par défaut
bool flash = false;
const int range = 1000;
const int freq = 1000; // frequence du hacheur

void translateIR() // Fonction reliant le signal à la fonction associée
// describing Remote IR codes
{
switch(results.value)
  {
  case ARRET: Serial.println("POWER         arrêt des moteurs avec mise à 0 des DCG et DCD"); // arrêt par la mise à 0 des variables DCG et DCD
    DCG = 0;
    DCD = 0;
    analogWrite(EnaD, DCD);         //  enaD  ENA
    analogWrite(EnaG, DCG);         //  enaG  ENB
    digitalWrite(fwdD, HIGH);       //  fwdD  IN1
    digitalWrite(bwdD, LOW);        //  bwdD  IN2
    digitalWrite(fwdG, HIGH);       //  fwdG  IN3  
    digitalWrite(bwdG, LOW);        //  bwdG  IN4   
    delay(5);
    break;
  
  case ACCELG: Serial.println("VOL+         ACCEL MOTEUR G"); 
    DCG = DCG + 1;
    analogWrite(EnaG, DCG);         //  enaG  ENB
    Serial.println(DCG);
    delay(5);
    break;

  case FREING: Serial.println("VOL-         FREIN MOTEUR G"); 
    DCG = DCG - 1;
    analogWrite(EnaG, DCG);         //  enaG  ENB
    Serial.println(DCG);
    delay(5);
    break;

  case VIREG: Serial.println("GAUCHE   VIREG"); //rotation à gauche pendant 150ms
    analogWrite(EnaG, 0);           //  enaG  ENA  motG à 0
    analogWrite(EnaD, DC);         //  enaD  ENB  rotation motD à DC
    digitalWrite(fwdG, HIGH);       //  fwdG  IN1
    digitalWrite(bwdG, LOW);        //  bwdG  IN2
    digitalWrite(fwdD, HIGH);       //  fwdD  IN3  
    digitalWrite(bwdD, LOW);        //  bwdD  IN4   
    delay(150);                   //  rot motD pendant 150ms
    analogWrite(EnaG, DCG);         //  enaG  ENA    motG à DCG
    analogWrite(EnaD, DCD);         //  enaD  ENB    motD à DCD
    break;
  
  case VIRED: Serial.println("DROITE   VIRED" ); //rotation à droite pendant 150ms
    analogWrite(EnaG, DC);         //  enaG  ENA   rotation motG à DC
    analogWrite(EnaD, 0);           //  enaD  ENB   motD à 0
    digitalWrite(fwdG, HIGH);       //  fwdG  IN1
    digitalWrite(bwdG, LOW);        //  bwdG  IN2
    digitalWrite(fwdD, HIGH);       //  fwdD  IN3  
    digitalWrite(bwdD, LOW);        //  bwdD  IN4   
    delay(150);                   //  rotation motG pendant 150ms
    analogWrite(EnaG, DCG);         //  enaG  ENA   motG à DCG
    analogWrite(EnaD, DCD);         //  enaD  ENB   motD à DCD
    break;
    
  case AVT1: Serial.println("HAUT            AVT1"); //  marche avant permanente avec DCG et DCD à DC
    DCG = DC;
    DCD = DC;
    analogWrite(EnaG, DCG);         //  enaG  ENA avance
    analogWrite(EnaD, DCD);         //  enaD  ENB avance
    Serial.println(DCG);
    digitalWrite(fwdG, HIGH);       //  fwdG  IN1
    digitalWrite(bwdG, LOW);        //  bwdG  IN2
    digitalWrite(fwdD, HIGH);       //  fwdD  IN3  
    digitalWrite(bwdD, LOW);        //  bwdD  IN4   
    delay(5);
    break;
    
  case ARR1: Serial.println("BAS            ARR1"); // marche arrière permanente avec DCG et DCD à DC
    DCG = DC;
    DCD = DC;
    analogWrite(EnaG, DCG);         //  enaG  ENA arrière à DCG
    analogWrite(EnaD, DCD);         //  enaD  ENB arrière à DCD
    digitalWrite(fwdG, LOW);         //  fwdG  IN1
    digitalWrite(bwdG, HIGH);        //  bwdG  IN2
    digitalWrite(fwdD, LOW);         //  fwdD  IN3  
    digitalWrite(bwdD, HIGH);        //  bwdD  IN4   
    delay(5);
    break;

  case PAGE: Serial.println("PAGE            AVT1000ms"); // avance pendant 1000ms à la valeur courante de DCG et DCD
    Serial.println(DCG);
    delay(5);
    analogWrite(EnaG, DCG);         //  enaG  ENA avance à DCG
    analogWrite(EnaD, DCD);         //  enaD  ENB avance à DCD
    digitalWrite(fwdG, HIGH);       //  fwdG  IN1
    digitalWrite(bwdG, LOW);        //  bwdG  IN2
    digitalWrite(fwdD, HIGH);       //  fwdD  IN3  
    digitalWrite(bwdD, LOW);        //  bwdD  IN4   
    delay(1000);                   // avance pendant 1000ms
    analogWrite(EnaG, 0);           //  enaG  ENA   motG à 0
    analogWrite(EnaD, 0);           //  enaD  ENB   motD à 0
  break;

  case HOME: Serial.println("HOME            AVT200ms"); // avance pendant 200ms à la valeur courante de DCG et DCD
    Serial.println(DCG);          // la valeur courante de DCG 
    analogWrite(EnaG, DCG);         //  enaG  ENA avance à DCG
    analogWrite(EnaD, DCD);         //  enaD  ENB avance à DCD
    digitalWrite(fwdG, HIGH);       //  fwdG  IN1
    digitalWrite(bwdG, LOW);        //  bwdG  IN2
    digitalWrite(fwdD, HIGH);       //  fwdD  IN3  
    digitalWrite(bwdD, LOW);        //  bwdD  IN4   
    delay(200);                   // avance pendant 200ms
    analogWrite(EnaG, 0);           //  enaG  ENA   motG à 0
    analogWrite(EnaD, 0);           //  enaD  ENB   motD à 0
  break;

  case OKOK: Serial.println("OK       arrêt des moteurs avec mise à 0 de DCG etDCD"); // arret des 2 moteurs, même fonction que le bouton rouge POWER 
    DCG = 0;
    DCD = 0;
    analogWrite(EnaG, DCG);         //  enaG  ENA
    analogWrite(EnaD, DCD);         //  enaD  ENB
    digitalWrite(fwdG, HIGH);       //  fwdG  IN1
    digitalWrite(bwdG, LOW);        //  bwdG  IN2
    digitalWrite(fwdD, HIGH);       //  fwdD  IN3  
    digitalWrite(bwdD, LOW);        //  bwdD  IN4   
    delay(5);
    break;
  
  case RETOUR: Serial.println("RETOUR         initialisation de DCG et DCD à DC"); // mettre DCG et DCD à DC
  DCG = DC;
  DCD = DC;
  break;

  case SOURIS: Serial.println("SOURIS         allumage/extinction led ESP32-cam");
    if (!flash){
      digitalWrite(cam, HIGH);          // Allumage
      flash = true;
    }else{
      digitalWrite(cam, LOW);         // Extinction
      flash = false;
    }
    
  break;

  default:
  Serial.println(" code inactif ");
  Serial.println(results.value, HEX);
  }
  delay(100); // Permet de laisser le temps de recevoir le prochain signal
}

void setup() {
  analogWriteRange(range);
  analogWriteFreq(freq);
  irrecv.enableIRIn(); // Initialise le récepteur IR
  //Init Serial USB
  Serial.begin(115200);
  Serial.println(F("Initialize System"));

  //Init DCmotor
  pinMode(EnaG, OUTPUT);
  pinMode(fwdG, OUTPUT);
  pinMode(bwdG, OUTPUT);

  pinMode(EnaD, OUTPUT);
  pinMode(fwdD, OUTPUT);
  pinMode(bwdD, OUTPUT);
  pinMode(cam,  OUTPUT);
  //Extinction de la LED de l'ESP32-cam
  digitalWrite(cam, LOW);
}

void loop() // Boucle qui s'éxécute à l'infini
{
  if (irrecv.decode(&results)) 
  {
    translateIR();
    irrecv.resume(); // Permet de recevoir la valeur suivante
  }
}
