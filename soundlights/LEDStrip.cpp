#include "LEDStrip.h"
#include "LEDEffect.h"


LEDStrip::LEDStrip() {
  
}

void LEDStrip::show() {
  LEDS.show();
}  

void LEDStrip::init() {
  LEDS.addLeds<NEOPIXEL, LED_DT>(leds, LED_COUNT).setTemperature(WarmFluorescent).setCorrection(0xFFFFFF);  // настройки для вашей ленты (ленты на WS2811, WS2812, WS2812B)
  LEDS.setBrightness(config.bright);


  updateColor(255,0,0); show(); 
  updateColor(0,255,0); show(); 
  updateColor(0,0,255); show();
  updateColor(0,0,0);show(); 
  switchEffect();
  
}

void LEDStrip::setConfig(LEDStrip_config newConfig)  {
  if (config.ledMode!=newConfig.ledMode) {
    Serial.print("New mode: ");
    Serial.println(newConfig.ledMode);
    config =newConfig;  
    switchEffect();
  }
  else {

    config =newConfig;  
    currentEffect->configChanged();
  }
  LEDS.setBrightness(config.bright);
}

void LEDStrip::switchEffect() {
 delete currentEffect  ;
   switch(config.ledMode){
      case 1: currentEffect= new LEDEffect_rainbow_fade(*this); break;
      case 2: currentEffect= new LEDEffect_rainbow_loop(*this); break;
      case 3: currentEffect= new LEDEffect_new_rainbow_loop(*this); break;

      case 7:  currentEffect= new LEDEffect_Fire(*this);  break; 

      case 8: 
      currentEffect= new LEDEffect_fixedColor(*this); break;      
      
     /*     
      case 2: rainbow_loop(); break;
      case 3: new_rainbow_loop();  break;
      case 4: random_march(); break;  
      case 5: rgb_propeller(); break;
      case 6: rotatingRedBlue();  break;
      
      case 8: blueFire(55, 250, delayValue); break;  
      case 9: random_burst(); break;
      case 10: flicker();  break;
      case 11: random_color_pop(); break;                                      
      case 12: Sparkle(255, 255, 255, delayValue); break;                   
      case 13: color_bounce(); hueValue = 0; break;
      case 14: color_bounceFADE();  hueValue = 0; break;
      case 15: red_blue_bounce();  hueValue = 0; break;
      case 16: rainbow_vertical(); stepValue = 15; break;
      case 17: matrix();  hueValue = 95; break; 
  
      // тяжелые эффекты
      case 18: rwb_march();  break;                         
      case 19: flame(); break;
      case 20: theaterChase(255, 0, 0, delayValue);  break;
      case 21: Strobe(r, g, b, 10, delayValue, 1000); break;
      case 22: policeBlinker();  break;
      case 23: kitt();  break;
      case 24: rule30();  break;
      case 25: fade_vertical(); hueValue = 180; break;
      case 26: fadeToCenter(); break;
      case 27: runnerChameleon(); break;
      case 28: blende(); break;
      case 29: blende_2();break;*/
      case 30: currentEffect= new LEDEffect_visuals(*this); break;
      case 31: currentEffect= new LEDEffect_FadeTest(*this);break;
      default:
      
          currentEffect= new LEDEffect_null(*this); break;
   }
}
 void LEDStrip::process(){ 
  
  currentEffect->process();

   
}

void LEDStrip::updateColor(uint8_t r,uint8_t g,uint8_t b){
  for(uint8_t i = 0 ; i < LED_COUNT; i++ ){
    leds[i] = CRGB(r,g,b);
  }
}
/*





*/
