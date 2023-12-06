#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN         D7  // pin connected to the NeoPixels?
#define NUMPIXELS   1   // nb of NeoPixels
#define NB_COLOR    4 // nombre de couleurs

#define ROUGE 1
#define JAUNE 2
#define BLANC 3
#define BLEU  4
#define VERT  5

class Neopix
{
private:
    Adafruit_NeoPixel* pixels = NULL;
    
public:
    Neopix(Adafruit_NeoPixel* pixels);   // constructeur
    void neo(int color);
};