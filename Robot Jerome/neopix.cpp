#include "neopix.h"

Neopix::Neopix(Adafruit_NeoPixel* pixels){
    this->pixels = pixels;
    this->pixels->begin();
    this->neo(1);
    delay(100);
    this->neo(2);
    delay(100);
    this->neo(3);
    delay(100);
    this->neo(-1);
    delay(100);
}

void Neopix::neo(int color){
    if (color < 0 ) color = 0;
    if (color > NB_COLOR) color= NB_COLOR;
    switch(color){
        case ROUGE :// rouge
            this->pixels->setPixelColor(0, this->pixels->Color(0, 150, 0)); break;
        case JAUNE ://jaune
            this->pixels->setPixelColor(0, this->pixels->Color(150, 250, 0)); break;
        case BLANC :// blanc
            this->pixels->setPixelColor(0, this->pixels->Color(150, 150, 150)); break;
        case BLEU :// bleu
            this->pixels->setPixelColor(0, this->pixels->Color(0, 0, 150)); break;
        case VERT ://vert
            this->pixels->setPixelColor(0, this->pixels->Color(150, 0, 0)); break;
  }
    this->pixels->show();   // Send the updated pixel colors to the hardware.
}