#include <Adafruit_NeoPixel.h>      //downloaded from https://github.com/adafruit/Adafruit_NeoPixel
#include <avr/power.h> 

#define SERIAL_SPEED       115200

#define REPORT_AFTER_TICKS 96       // can't be set to number 16 or smaller, needs at least some ticks to work properly

#define PORT_MICROPHONE    A0
#define PORT_LIGHT         A1
#define PORT_TEMPERATURE   A2

#define RGB_PIN            6
#define RGB_LEDS           300
#define RGB_BRIGHT         5        // 1=maximum brightness, 8=minimum brightness

#define MIC_CLAP_THRESH    250

#define MIC_TOP            1024
#define MIC_BOTTOM         0

#define LIGHT_TOP          700
#define LIGHT_BOTTOM       0

#define TEMP_TOP           500
#define TEMP_BOTTOM        400

#define MAX_INT            65535


Adafruit_NeoPixel strip = Adafruit_NeoPixel(RGB_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

uint8_t       *pixels;
unsigned char  globalR;
unsigned char  globalG;
unsigned char  globalB;
unsigned char  brigthness = RGB_BRIGHT;   

void setup() {
  //init serial
  Serial.begin(SERIAL_SPEED);

  //init RGB
  strip.begin();
  strip.show();
  pixels=strip.getPixels();
}


void loop() {
  static unsigned int  valueMicrophone  = 0;
  static unsigned int  minMicrophone    = MAX_INT;
  static unsigned int  maxMicrophone    = 0;
  static unsigned int  valueLight       = 0;
  static unsigned long sumLight         = 0;
  static unsigned int  valueTemperature = 0;
  static unsigned long sumTemperature   = 0;
  static unsigned int  tick             = 0;
  
  valueMicrophone  = analogRead(PORT_MICROPHONE );
  valueLight       = analogRead(PORT_LIGHT      );
  valueTemperature = analogRead(PORT_TEMPERATURE);

  if (valueMicrophone < minMicrophone) {
    minMicrophone = valueMicrophone;
  }

  if (valueMicrophone > maxMicrophone) {
    maxMicrophone = valueMicrophone;
  }

  sumTemperature += valueTemperature;
  sumLight       += valueLight;

  if ((tick % REPORT_AFTER_TICKS) == 0 ) {
    //output data for the processing GUI once in while

    sumLight       = constrain(sumLight       / REPORT_AFTER_TICKS, LIGHT_BOTTOM, LIGHT_TOP);
    sumTemperature = constrain(sumTemperature / REPORT_AFTER_TICKS,  TEMP_BOTTOM, TEMP_TOP );
    
    Serial.print(maxMicrophone - minMicrophone);
    Serial.print(',');
    Serial.print(  map(sumLight,       LIGHT_BOTTOM, LIGHT_TOP, 0, 100));
    Serial.print(',');
    Serial.println(map(sumTemperature, TEMP_BOTTOM,  TEMP_TOP,  0, 100));  

    if ((maxMicrophone - minMicrophone) > MIC_CLAP_THRESH) {
      brigthness = 1;
    }
    else {
      brigthness = RGB_BRIGHT;
    }

    minMicrophone    = MAX_INT;
    maxMicrophone    = 0;
    sumTemperature   = 0;
    sumLight         = 0;
  }

  // delay in time the led movement event, 
  // this allows some ADC readings be done in between and not having large gaps in the ADC readings
  // more frequent but smaller gaps are better
  if ((tick % REPORT_AFTER_TICKS) == 8 ) {
    rainbow();
  }

  // delay in time the led display refresh
  if ((tick % REPORT_AFTER_TICKS) == 16 ) {
    strip.show();
  }
  
  tick++;
}


//shift all leds by 1 and update first led
void rainbow() {
  static unsigned int i;

  for (unsigned int j=strip.numPixels(); j>0; j--) {
    unsigned int offset=j*3;
    pixels[offset+0] = pixels[offset-3];
    pixels[offset+1] = pixels[offset-2];
    pixels[offset+2] = pixels[offset-1];
  }

  wheel(((i * 256 / strip.numPixels()) ) & 255);

  pixels[0]=globalG;
  pixels[1]=globalR;
  pixels[2]=globalB;
  
  i++;
}

//Trying to display all the colors https://en.wikipedia.org/wiki/HSL_and_HSV
void wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    globalR = (85 - WheelPos)*3 >> brigthness;
    globalG = 0;
    globalB = WheelPos*3 >> brigthness;
    return;
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    globalR = 0;
    globalG = WheelPos*3 >> brigthness;
    globalB = (85 - WheelPos)*3 >> brigthness;
    return;
  }
  WheelPos -= 170;
  globalR = WheelPos*3 >> brigthness;
  globalG = (85 - WheelPos)*3 >> brigthness;
  globalB = 0;
}

