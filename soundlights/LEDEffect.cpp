
#include "LEDStrip.h"
#include "LEDEffect.h"

#include "inlinetimer.h" 

LEDEffect::LEDEffect(LEDStrip &strip) : strip(strip) {
  
  configChanged();  
}

void LEDEffect::process() {
  if (millis()<nextmillis){
    show(); 
    return;
  }

  nextFrame(strip.getConfig().delayValue);
  render();
}

void LEDEffect_fixedColor::configChanged() {
  render();
}

void LEDEffect_fixedColor::render() {
  strip.updateColor(strip.getConfig().r,strip.getConfig().g,strip.getConfig().b);
 show(20);

}

void LEDEffect_null::render() {
  strip.updateColor(0,0,0);
  show(20);

}


#define INTENSITY_POS LED_COUNT
#define BURST_POS LED_COUNT+1
#define SPEED_POS LED_COUNT+2
WiFiUDP udp;

LEDEffect_visuals::LEDEffect_visuals(LEDStrip &strip) : LEDEffect(strip){
  udp.begin(localPort);
 
  memset(packetBuffer,20,BURST_POS);

  packetBuffer[SPEED_POS] =100;
}

LEDEffect_visuals::~LEDEffect_visuals() {
  udp.stop();
}
void LEDEffect_visuals::render() {
   
  inlineTimer(VIS_FRAME_RATE_DELAY , {

    int cb = udp.parsePacket();

    if (cb) 
    {
      memset(packetBuffer, 0, sizeof(packetBuffer));
      udp.read(packetBuffer, sizeof(packetBuffer)); // read the packet into the buffer, we are reading only one byt
    }

  });
   

  strip.setBrightness(255);
  //Serial.println(packetBuffer);

  if (packetBuffer[BURST_POS]>=1) {
    strip.setBrightness(255);
    for (int i =0 ;i<LED_COUNT;i++) {
      strip.leds[i] = CRGB::White;
    }
    packetBuffer[BURST_POS]--;
  }
  else {

    for (int i =0 ;i<LED_COUNT;i++) {
      strip.leds[i] = CHSV(b_hue+i/5,255, packetBuffer[i]);
    }
  }

  show(20);    

  inlineTimer(packetBuffer[SPEED_POS], {
    b_hue+=1;
  });    

}


//плавная смена цветов
void LEDEffect_rainbow_fade::render(){ 
  ihue++;
  if(ihue > 255){
    ihue = 0;
  }
  for(int idex = 0 ; idex < LED_COUNT; idex++ ){
    strip.leds[idex] = CHSV(ihue, saturationVal, 255);
  }
  show();
}



void LEDEffect_FadeTest::render() {

  for (int i =0 ;i<LED_COUNT;i++) {
    switch (i%3){
      case 0:
        strip.leds[i] = CRGB::Red;
        break;
      case 1:
        strip.leds[i] = CRGB::Green;
        break;
      case 2:
        strip.leds[i] = CRGB::Blue;
        break;
    }
  }

  strip.setBrightness(brightness);
  show();

  brightness+=fadeout;
    
  if (brightness==0){
    fadeout=1;
  }
  else if (brightness==255){
    fadeout=-1;
  }

}


// крутящаяся радуга
void LEDEffect_rainbow_loop::render(){ 
  idex++;
  ihue = ihue + stepValue;
  if(idex >= LED_COUNT){
    idex = 0;
  }
  if(ihue > 255){
    ihue = 0;
  }
  strip.leds[idex] = CHSV(ihue, saturationVal, 255);
  show();
 
}
/*
// случайная смена цветов
void LEDEffect_random_burst::render(){
  idex = random(0, LED_COUNT);
  ihue = random(0, 255);
  strip.leds[idex] = CHSV(ihue, saturationVal, 255);
  show();
  nextFrame (strip.getConfig().delayValue);    
}

// бегающий пиксель
void LEDEffect_color_bounce::render(){
  if(bouncedirection == 0){
    idex = idex + 1;
    if(idex == LED_COUNT){
      bouncedirection = 1;
      idex = idex - 1;
    }
  }
  if(bouncedirection == 1){
    idex = idex - 1;
    if(idex == 0){
      bouncedirection = 0;
    }
  }
  for(int i = 0; i < LED_COUNT; i++ ){
    if(i == idex){
      strip.leds[i] = CHSV(hueValue, saturationVal, 255);
    }
    else{
      strip.leds[i] = CHSV(0, 0, 0);
    }
  }
  show();
  
}

//бегающий паровозик пикселей
void LEDEffect_color_bounceFADE::render(){
  if(bouncedirection == 0){
    idex = idex + 1;
    if(idex == LED_COUNT){
      bouncedirection = 1;
      idex = idex - 1;
    }
  }
  if(bouncedirection == 1){
    idex = idex - 1;
    if(idex == 0){
      bouncedirection = 0;
    }
  }
  int iL1 = adjacent_cw(idex);
  int iL2 = adjacent_cw(iL1);
  int iL3 = adjacent_cw(iL2);
  int iR1 = adjacent_ccw(idex);
  int iR2 = adjacent_ccw(iR1);
  int iR3 = adjacent_ccw(iR2);

  for(int i = 0; i < LED_COUNT; i++ ){
    if(i == idex){
      strip.leds[i] = CHSV(hueValue, saturationVal, 255);
    }
    else if(i == iL1){
      strip.leds[i] = CHSV(hueValue, saturationVal, 150);
    }
    else if(i == iL2){
      strip.leds[i] = CHSV(hueValue, saturationVal, 80);
    }
    else if(i == iL3){
      strip.leds[i] = CHSV(hueValue, saturationVal, 20);
    }
    else if(i == iR1){
      strip.leds[i] = CHSV(hueValue, saturationVal, 150);
    }
    else if(i == iR2){
      strip.leds[i] = CHSV(hueValue, saturationVal, 80);
    }
    else if(i == iR3){
      strip.leds[i] = CHSV(hueValue, saturationVal, 20);
    }
    else{
      strip.leds[i] = CHSV(0, 0, 0);
    }
  }
  show();
  
}

// вращение красного и синего
void LEDEffect_red_blue_bounce::render(){
  idex++;
  if(idex >= LED_COUNT){
    idex = 0;
  }
  int idexR = idex;
  int idexB = antipodal_index(idexR);
  int thathue =(hueValue + 160) % 255;

  for(int i = 0; i < LED_COUNT; i++ ){
    if(i == idexR){
      strip.leds[i] = CHSV(hueValue, saturationVal, 255);
    }
    else if(i == idexB){
      strip.leds[i] = CHSV(thathue, saturationVal, 255);
    }
    else{
      strip.leds[i] = CHSV(0, 0, 0);
    }
  }
  show();
  
}

int LEDEffect_antipodal_index::render(){
  int iN = i + TOP_INDEX;
  if(i >= TOP_INDEX){
    iN =( i + TOP_INDEX ) % LED_COUNT;
  }
  return iN;
}

// вращение красного/синего
void LEDEffect_rotatingRedBlue::render(){
  idex++;
  if(idex >= LED_COUNT){
    idex = 0;
  }
  int idexR = idex;
  int idexB = antipodal_index(idexR);
  int thathue =(hueValue + 160) % 255;
  strip.leds[idexR] = CHSV(hueValue, saturationVal, 255);
  strip.leds[idexB] = CHSV(thathue, saturationVal, 255);
  show();
  
}

// случайный стробоскоп
void LEDEffect_flicker::render(){
  int random_bright = random(0, 255);
  int random_delay = random(10, 100);
  int random_bool = random(0, random_bright);
  if(random_bool < 10){
    for(int i = 0 ; i < LED_COUNT; i++ ){
      strip.leds[i] = CHSV(160, 50, random_bright);
    }
    show();saturationVal
void LEDEffect_fade_vertical(){
  idex++;
  if(idex > TOP_INDEX){
    idex = 0;
  }
  int idexA = idex;
  int idexB = horizontal_index(idexA);
  ibright = ibright + 10;
  if(ibright > 255){
    ibright = 0;
  }
  strip.leds[idexA] = CHSV(hueValue, saturationVal, ibright);
  strip.leds[idexB] = CHSV(hueValue, saturationVal, ibright);
  show();
  
}

//служебная функция
int LEDEffect_horizontal_index::render(){
  if(i == 0){
    return 0;
  }
  if(i == TOP_INDEX && EVENODD == 1){
    return TOP_INDEX + 1;
  }
  if(i == TOP_INDEX && EVENODD == 0){
    return TOP_INDEX;
  }
  return LED_COUNT - i;
}

//служебная функция
void LEDEffect_random_red::render(){
  int temprand;
  for(int i = 0; i < LED_COUNT; i++ ){
    temprand = random(0, 100);
    if(temprand > 50){
      strip.leds[i].r = 255;
    }
    if(temprand <= 50){
      strip.leds[i].r = 0;
    }
    strip.leds[i].b = 0; strip.leds[i].g = 0;
  }
  show();
}

//безумие красных светодиодов
void LEDEffect_rule30::render(){
  if(bouncedirection == 0){
    random_red();
    bouncedirection = 1;
  }
  copy_led_array();
  int iCW;
  int iCCW;
  int y = 100;
  for(int i = 0; i < LED_COUNT; i++ ){
    iCW = adjacent_cw(i);
    iCCW = adjacent_ccw(i);
    if(ledsX[iCCW][0] > y && strip.ledsX[i][0] > y && strip.ledsX[iCW][0] > y){
      strip.leds[i].r = 0;
    }
    if(ledsX[iCCW][0] > y && strip.ledsX[i][0] > y && strip.ledsX[iCW][0] <= y){
      strip.leds[i].r = 0;
    }
    if(ledsX[iCCW][0] > y && strip.ledsX[i][0] <= y && strip.ledsX[iCW][0] > y){
      strip.leds[i].r = 0;
    }
    if(ledsX[iCCW][0] > y && strip.ledsX[i][0] <= y && strip.ledsX[iCW][0] <= y){
      strip.leds[i].r = 255;
    }
    if(ledsX[iCCW][0] <= y && strip.ledsX[i][0] > y && strip.ledsX[iCW][0] > y){
      strip.leds[i].r = 255;
    }
    if(ledsX[iCCW][0] <= y && strip.ledsX[i][0] > y && strip.ledsX[iCW][0] <= y){
      strip.leds[i].r = 255;
    }
    if(ledsX[iCCW][0] <= y && strip.ledsX[i][0] <= y && strip.ledsX[iCW][0] > y){
      strip.leds[i].r = 255;
    }
    if(ledsX[iCCW][0] <= y && strip.ledsX[i][0] <= y && strip.ledsX[iCW][0] <= y){
      strip.leds[i].r = 0;
    }
  }
  show();
  
}
int LEDEffect_adjacent_cw::render(){
  int r;
  if(i < LED_COUNT - 1){
    r = i + 1;
  }else{
    r = 0;
  }
  return r;
}
int LEDEffect_adjacent_ccw(){
  int r;
  if(i > 0){
    r = i - 1;
  }
  else{
    r = LED_COUNT - 1;
  }
  return r;
}

// безумие случайных цветов
void LEDEffect_random_march::render(){
  copy_led_array();
  int iCCW;
  strip.leds[0] = CHSV(random(0, 255), 255, 255);
  for(int idex = 1; idex < LED_COUNT ; idex++ ){
    iCCW = adjacent_ccw(idex);
    strip.leds[idex].r = strip.ledsX[iCCW][0];
    strip.leds[idex].g = strip.ledsX[iCCW][1];
    strip.leds[idex].b = strip.ledsX[iCCW][2];
  }
  show();
   nextFrame (strip.getConfig().delayValue);
}

void LEDEffect_copy_led_array::render(){
  for(int i = 0; i < LED_COUNT; i++ ){
    strip.ledsX[i][0] = strip.leds[i].r;
    strip.ledsX[i][1] = strip.leds[i].g;
    strip.ledsX[i][2] = strip.leds[i].b;
  }
}

//белый синий красный бегут по кругу
void LEDEffect_rwb_march::render(){ 
  copy_led_array();
  int iCCW;
  idex++;
  if(idex > 2){
    idex = 0;
  }
  switch(idex){
    case 0:
      strip.leds[0].r = 255;
      strip.leds[0].g = 0;
      strip.leds[0].b = 0;
      break;
    case 1:
      strip.leds[0].r = 255;
      strip.leds[0].g = 255;
      strip.leds[0].b = 255;
      break;
    case 2:
      strip.leds[0].r = 0;
      strip.leds[0].g = 0;
      strip.leds[0].b = 255;
      break;
  }
  for(int i = 1; i < LED_COUNT; i++ ){
    iCCW = adjacent_ccw(i);
    strip.leds[i].r = strip.ledsX[iCCW][0];
    strip.leds[i].g = strip.ledsX[iCCW][1];
    strip.leds[i].b = strip.ledsX[iCCW][2];
  }
  show();
  
}

// эффект пламени
void LEDEffect_flame::render(){
  int idelay = random(0, 35);
  float hmin = 0.1; float hmax = 45.0;
  float hdif = hmax - hmin;
  int randtemp = random(0, 3);
  float hinc =(hdif / float(TOP_INDEX)) + randtemp;
  int ihue = hmin;
  for(int i = 0; i <= TOP_INDEX; i++ ){
    ihue = ihue + hinc;
    strip.leds[i] = CHSV(ihue, saturationVal, 255);
    int ih = horizontal_index(i);
    strip.leds[ih] = CHSV(ihue, saturationVal, 255);
    strip.leds[TOP_INDEX].r = 255; strip.leds[TOP_INDEX].g = 255; strip.leds[TOP_INDEX].b = 255;
    show();
  
  }
}

// радуга
void LEDEffect_rainbow_vertical::render(){ 
  idex++;
  if(idex > TOP_INDEX){
    idex = 0;
  }
  ihue = ihue + stepValue;
  if(ihue > 255){
    ihue = 0;
  }
  int idexA = idex;
  int idexB = horizontal_index(idexA);
  strip.leds[idexA] = CHSV(ihue, saturationVal, 255);
  strip.leds[idexB] = CHSV(ihue, saturationVal, 255);
  show();
  nextFrame (strip.getConfig().delayValue);    
}

// безумие случайных вспышек
void LEDEffect_random_color_pop::render(){
  idex = random(0, LED_COUNT);
  ihue = random(0, 255);
  updateColor(0, 0, 0);
  strip.leds[idex] = CHSV(ihue, saturationVal, 255);
  show();
  
}

// полицейская мигалка 
void LEDEffect_policeBlinker::render(){
  int hueValue = 0;
  int thathue =(hueValue + 160) % 255;
  for(int x = 0 ; x < 5; x++ ){
    for(int i = 0 ; i < TOP_INDEX; i++ ){
      strip.leds[i] = CHSV(hueValue, saturationVal, 255);
    }
    show(); 
    updateColor(0, 0, 0);
    show(); 
  }
  for(int x = 0 ; x < 5; x++ ){
    for(int i = TOP_INDEX ; i < LED_COUNT; i++ ){
      strip.leds[i] = CHSV(thathue, saturationVal, 255);
    }
    show(); 
    updateColor(0, 0, 0);
    show(); 
  }
}

void LEDEffect_rgb_propeller::render(){ // пропеллер
  idex++;
  int ghue =(hueValue + 80) % 255;
  int bhue =(hueValue + 160) % 255;
  int N3  = int(LED_COUNT / 3);
  int N6  = int(LED_COUNT / 6);
  int N12 = int(LED_COUNT / 12);

  for(int i = 0; i < N3; i++ ){
    int j0 =(idex + i + LED_COUNT - N12) % LED_COUNT;
    int j1 =(j0 + N3) % LED_COUNT;
    int j2 =(j1 + N3) % LED_COUNT;
    strip.leds[j0] = CHSV(hueValue, saturationVal, 255);
    strip.leds[j1] = CHSV(ghue, saturationVal, 255);
    strip.leds[j2] = CHSV(bhue, saturationVal, 255);
  }
  show();
  
}

//случайные вспышки красного
void LEDEffect_kitt::render(){
  int rand = random(0, TOP_INDEX);
  for(int i = 0; i < rand; i++ ){
    strip.leds[TOP_INDEX + i] = CHSV(hueValue, saturationVal, 255);
    strip.leds[TOP_INDEX - i] = CHSV(hueValue, saturationVal, 255);
    show();
    nextFrame (delayValue / rand);
  }
  for(int i = rand; i > 0; i-- ){
    strip.leds[TOP_INDEX + i] = CHSV(hueValue, saturationVal, 0);
    strip.leds[TOP_INDEX - i] = CHSV(hueValue, saturationVal, 0);
    show();
    nextFrame (delayValue / rand);
  }
}

// зеленые бегают по кругу случайно
void LEDEffect_matrix::render(){
  int rand = random(0, 100);
  if(rand > 90){
    strip.leds[0] = CHSV(hueValue, saturationVal, 255);
  }
  else{
    strip.leds[0] = CHSV(hueValue, saturationVal, 0);
  }
  copy_led_array();
  for(int i = 1; i < LED_COUNT; i++ ){
    strip.leds[i].r = strip.ledsX[i - 1][0];
    strip.leds[i].g = strip.ledsX[i - 1][1];
    strip.leds[i].b = strip.ledsX[i - 1][2];
  }
  show();
  
}
*/
// плавная вращающаяся радуга
void LEDEffect_new_rainbow_loop::render(){
  ihue -= 1;
  fill_rainbow( strip.leds, LED_COUNT, ihue );
  show();
  nextFrame (strip.getConfig().delayValue);    
}/*

void LEDEffect_setPixel(int Pixel, byte red, byte green, byte blue){
  strip.leds[Pixel].r = red;
  strip.leds[Pixel].g = green;
  strip.leds[Pixel].b = blue;
}
//служебная функция
void LEDEffect_setAll(byte red, byte green, byte blue){
  for(int i = 0; i < LED_COUNT; i++ ){
    setPixel(i, red, green, blue);
  }
  FastLED.show();
}
*/
//линейный огонь
#define MAXFLAME_VAL 55
LEDEffect_Fire::LEDEffect_Fire(LEDStrip &strip) : LEDEffect(strip) {
    for(int i = 0; i < LED_COUNT; i++){
      heat[i] = 0;
      if (random(0,40)==1)
      heat[i] = MAXFLAME_VAL;
    }
}

void LEDEffect_Fire::render(){



  for(int i = 0; i < LED_COUNT; i++){
     if (heat[i]<=10) 
    if (random(0,40)==1)
    heat[i] = MAXFLAME_VAL;
  }

  for(int i = 0; i < LED_COUNT; i++){
    if (heat[i]>10) {
      heat[i]--;
      strip.leds[i] = CHSV(heat[i],255,255);
    }

    else
    strip.leds[i] = CHSV(random(5,10),255,255);
  } 


  show();
  nextFrame(strip.getConfig().delayValue);
}/*

void LEDEffect_setPixelHeatColor::render(int Pixel, byte temperature){
  byte t192 = round((temperature / 255.0) * 191);
  byte heatramp = t192 & 0x3F;
  heatramp <<= 2;

  if( t192 > 0x80){
    setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ){
    setPixel(Pixel, 255, heatramp, 0);
  } else{
    setPixel(Pixel, heatramp, 0, 0);
  }
}

void LEDEffect_CenterToOutside::render(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
  for(int i =((LED_COUNT - EyeSize) / 2); i >= 0; i--){
    setAll(0, 0, 0);

    setPixel(i, red / 10, green / 10, blue / 10);
    for(int j = 1; j <= EyeSize; j++){
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(LED_COUNT - i, red / 10, green / 10, blue / 10);
    for(int j = 1; j <= EyeSize; j++){
      setPixel(LED_COUNT - i - j, red, green, blue);
    }
    setPixel(LED_COUNT - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    FastLED.show();
    nextFrame(SpeedDelay);
  }
  nextFrame(ReturnDelay);
}

void LEDEffect_utsideToCenter::render(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
  for(int i = 0; i <=((LED_COUNT - EyeSize) / 2); i++){
    setAll(0, 0, 0);

    setPixel(i, red / 10, green / 10, blue / 10);
    for(int j = 1; j <= EyeSize; j++){
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(LED_COUNT - i, red / 10, green / 10, blue / 10);
    for(int j = 1; j <= EyeSize; j++){
      setPixel(LED_COUNT - i - j, red, green, blue);
    }
    setPixel(LED_COUNT - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    FastLED.show();
    nextFrame(SpeedDelay);
  }

}

void LEDEffect_LeftToRight::render(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
  for(int i = 0; i < LED_COUNT - EyeSize - 2; i++){
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    for(int j = 1; j <= EyeSize; j++){
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    FastLED.show();
    nextFrame(SpeedDelay);
  }
  nextFrame(ReturnDelay);
}

// случайные вспышки белого цвета
void LEDEffect_Sparkle::render(byte red, byte green, byte blue, int SpeedDelay){
  int Pixel = random(LED_COUNT);
  setPixel(Pixel, red, green, blue);
  FastLED.show();
  nextFrame(SpeedDelay);
  setPixel(Pixel, 0, 0, 0);
}

// бегущие каждые 3 пикселя
void LEDEffect_theaterChase::render(byte red, byte green, byte blue, int SpeedDelay){
  for(int j = 0; j < 10; j++){
    for(int q = 0; q < 3; q++){
      for(int i = 0; i < LED_COUNT; i = i + 3){
        setPixel(i + q, red, green, blue);
      }
      FastLED.show();
      delay(SpeedDelay);
      for(int i = 0; i < LED_COUNT; i = i + 3){
        setPixel(i + q, 0, 0, 0);
      }
    }
  }
}

// стробоскопический эффект
void LEDEffect_Strobe::render(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){

  static int count = 0;

  if ((count%2)==0) {
    

    setAll(red, green, blue);
    FastLED.show();
    nextFrame (FlashDelay);
  }
  else{    
    setAll(0, 0, 0);
    FastLED.show();
    nextFrame (FlashDelay);
  }
  count++;
  if (StrobeCount==count) {
    count=0;
    nextFrameEndPause;
  }

}

//голубой линейный огонь
void LEDEffect_blueFire::render(int Cooling, int Sparking, int SpeedDelay){
  static byte heat[LED_COUNT];
  int cooldown;

  for(int i = 0; i < LED_COUNT; i++){
    cooldown = random(0,((Cooling * 10) / LED_COUNT) + 2);

    if(cooldown > heat[i]){
      heat[i] = 0;
    } else{
      heat[i] = heat[i] - cooldown;
    }
  }
  for( int k = LED_COUNT - 1; k >= 2; k--){
    heat[k] =(heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
  if( random(255) < Sparking ){
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
  }
  for( int j = 0; j < LED_COUNT; j++){
    setPixelHeatColorBlue(j, heat[j] );
  }
  FastLED.show();
  delay(SpeedDelay);
}

void LEDEffect_setPixelHeatColorBlue::render(int Pixel, byte temperature){
  byte t192 = round((temperature / 255.0) * 191);
  byte heatramp = t192 & 0x03;
  heatramp <<= 2;

  if( t192 > 0x03){
    setPixel(Pixel, heatramp,255, 255);
  } else if( t192 > 0x40 ){
    setPixel(Pixel, 255, heatramp, 0);
  } else{
    setPixel(Pixel, 0, 0, heatramp);
  }
}

//расплывающие цвета
void LEDEffect_fadeToCenter::render(){
 static uint8_t hue;
  for(int i = 0; i < LED_COUNT/2; i++) {   
    strip.leds.fadeToBlackBy(40);
    strip.leds[i] = CHSV(hue++,255,255);
    strip.leds(LED_COUNT/2,LED_COUNT-1) = strip.leds(LED_COUNT/2 - 1 ,0);
    FastLED.delay(33);
  }  
}

//бегающий по кругу паровозик
void LEDEffect_runnerChameleon::render(){
    static uint8_t hue = 0;
  for(int i = 0; i < LED_COUNT; i++) {
    strip.leds[i] = CHSV(hue++, 255, 255);
    FastLED.show(); 
     strip.leds[i] = CRGB::Black;
//    fadeall();
    delay(10);
  }
}


void LEDEffect_blende::render(){
    static uint8_t hue = 0;
  for(int i = 0; i < LED_COUNT; i++) {
    strip.leds[i] = CHSV(hue++, 255, 255);
    FastLED.show(); 
//     strip.leds[i] = CRGB::Black;
    fadeall();
    delay(10);
  }

  for(int i = (LED_COUNT)-1; i >= 0; i--) {
    strip.leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
//     strip.leds[i] = CRGB::Black;
    fadeall();
    delay(10);
  }
}

void LEDEffect_blende_2::render(){
    static uint8_t hue = 0;
  for(int i = 0; i < LED_COUNT; i++) {
    strip.leds[i] = CHSV(hue++, 255, 255);
    FastLED.show(); 
    strip.leds[i] = CRGB::Black;
    fadeall();
    delay(10);
  }

  for(int i = (LED_COUNT)-1; i >= 0; i--) {
    strip.leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
//     strip.leds[i] = CRGB::Black;
    fadeall();
    delay(10);
  }
}
//служебная функция
void LEDEffect_fadeall::render(){
  for(int i = 0; i < LED_COUNT; i++) {
    strip.leds[i].nscale8(250); 
  } 
}

*/
