
#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_INTERRUPT_RETRY_COUNT 0

#define VIS_FRAME_RATE 24

#define VIS_FRAME_RATE_DELAY 1000/VIS_FRAME_RATE



#ifndef LEDSTRIP_H
#define LEDSTRIP_H
class LEDEffect;
#include <stdint.h> 

#include <FastLED.h>


#define LED_COUNT 144 // число пикселей в ленте
#define LED_DT 5    // пин, куда подключен DIN ленты (номера пинов ESP8266 совпадает с Arduino)  



struct LEDStrip_config {
  uint8_t bright = 10; // яркость (0 - 255)
  uint8_t ledMode = 0; // эффект (0 - 29)
  int r=0,g=0,b=0;
  uint8_t delayValue = 10; // задержка
};

class LEDStrip {
    
  LEDEffect * currentEffect=NULL;
  LEDStrip_config config;
  uint8_t flag = 1; // флаг отмены эффекта

  

  
  uint8_t stepValue = 1; // шаг по пикселям
  uint8_t hueValue = 0; // тон цвета

  
  unsigned long nextmillis=0;

  uint8_t idex = 0; //индекс текущего пикселя    
  uint8_t ihue = 0; // тон цвета
  uint8_t saturationVal = 255; // насыщенность 
  uint8_t ibright = 0; //значение яркости
  uint16_t TOP_INDEX = uint8_t(LED_COUNT / 2); // получаем середину ленты
  uint8_t EVENODD = LED_COUNT % 2; //флаг проверки четности
  uint8_t bouncedirection = 0; //флаг для color_bounce()
  int ledsX[LED_COUNT][3]; //массив для сохранения случайных значений пикселя 
  
    
    
    void fixedColor();

    void switchEffect();
  public:
    CRGBArray<LED_COUNT> leds;
    LEDStrip();
    void init();
    void process();
    void updateColor(uint8_t r,uint8_t g,uint8_t b);
    void show()    ;
    void setBrightness(int b) { LEDS.setBrightness(b);};
    LEDStrip_config getConfig(){ return config;};

    void setConfig(LEDStrip_config) ;
};


#endif