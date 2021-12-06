
#ifndef LEDEFFECT_H
#define LEDEFFECT_H

class LEDStrip;

#include <WiFiUdp.h>

#include "LEDStrip.h"
class LEDEffect {
  protected:
    int saturationVal =255;
    
    unsigned long nextmillis=0;
    LEDStrip& strip;

  public:
  LEDEffect(LEDStrip &strip);
  void process();
  virtual void configChanged(){};
  virtual void render(){ nextFrame(1000); };

  void nextFrame(unsigned long delay) { nextmillis = millis() +delay; };
  void show(int nextFrame = -1) {
    strip.show(); 
    if (nextFrame>-1) { 
      nextmillis = millis() + nextFrame;
    };
  };
};

class LEDEffect_fixedColor : public LEDEffect {

  public:
  LEDEffect_fixedColor(LEDStrip &strip) : LEDEffect(strip) {} ;
  void render() ;
  void configChanged();
};

class LEDEffect_null : public LEDEffect {

  public:
  LEDEffect_null(LEDStrip &strip) : LEDEffect(strip) {} ;
  void render() ;

};



class LEDEffect_visuals : public LEDEffect {

  unsigned long nexthuemillis=0;
 

  char packetBuffer[LED_COUNT+3]; 
  uint8_t b_hue=0;

  unsigned long nextpackagemillis=0;
  unsigned int localPort = 12345; // local port to listen for UDP packets
  

  public:
    LEDEffect_visuals(LEDStrip &strip)  ;
    ~LEDEffect_visuals()  ;
    void render() ;

};

class LEDEffect_rainbow_fade : public LEDEffect {
  int ihue=0;
  
  void render();
  public:
    LEDEffect_rainbow_fade(LEDStrip &strip) : LEDEffect(strip) {}  ;
};


class LEDEffect_FadeTest : public LEDEffect {
  int fadeout=1;
  int brightness = 0;  
  void render();
  public:
    LEDEffect_FadeTest(LEDStrip &strip) : LEDEffect(strip) {}  ;
};

class LEDEffect_rainbow_loop : public LEDEffect {
  int idex=0;
  int ihue  =0;
  int stepValue=20;
  void render();
  public:
    LEDEffect_rainbow_loop(LEDStrip &strip) : LEDEffect(strip) {}  ;
};

class LEDEffect_new_rainbow_loop : public LEDEffect {

  int ihue  =0;

  void render();
  public:
    LEDEffect_new_rainbow_loop(LEDStrip &strip) : LEDEffect(strip) {}  ;
};

class LEDEffect_Fire : public LEDEffect {


  byte heat[LED_COUNT];
  int cooldown;


  void render();
  public:
    LEDEffect_Fire(LEDStrip &strip)  ;
};



#endif