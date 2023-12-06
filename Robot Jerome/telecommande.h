//#define DEBUG
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// codes télécommande
#define UP    "1FE1CE3"
#define RIGHT "1FE9C63"
#define DOWN  "1FE02FD"
#define LEFT  "1FEEC13"
#define ENTER "1FEC837"
#define BACK  "1FEE41B"
#define PLUS  "1FEE11E"
#define MOINS "1FE916E"
#define MOUSE "1FE12ED"
#define HOME  "1FECE31"
#define ONOFF "1FE817E"

class Telecommande
{
private:
  IRrecv* recv = NULL;
public:
  Telecommande(IRrecv* recv);   // constructeur
  String read();
  decode_results results;
};
