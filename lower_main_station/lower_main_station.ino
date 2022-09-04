/*
  ----------------------------Code of the Lightning of the stairs----------------------------

  App project setup:
    RTC widget (no pin required)
    Value Display widget on V1
    Value Display widget on V2

  WARNING :
  For this example you'll need Time keeping library:
    https://github.com/PaulStoffregen/Time

  This code is based on an example from the Time library:
    https://github.com/PaulStoffregen/Time/blob/master/examples/TimeSerial/TimeSerial.ino
 *************************************************************/

/* Config PIR filter
  //#define pinPIR1 D2                                             // The PIR1 sensor pin
  const int calibrationTime = 30;                                    // the time we give the sensor to calibrate (10-60 secs according to the datasheet)
  volatile int candidatePIR1;                                        // the un-filtered new status candidate of the PIR sensor
  int statusPIR1 = HIGH;                                             // the noise/spike-filtered status of the PIR sensor
  volatile boolean changePIR1 = false;                               // the indication that a potential status change has happened
  volatile long unsigned int changeTimePIR1 = 0;                     // the time stamp of the potential status change (edge timer start)
  const long unsigned int delayNoise = 1000;                          // milliseconds. Status changes shorter than this are deemed noise/spike. Test to find the best value. 100ms works for me.
  //End PIR
*/
//#include <przejscia.h>
#include "FastLED.h"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <Adafruit_NeoPixel.h>
#define PIN D1
#define NUMPIXELS 320
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
char auth[] = "token";
uint32_t white = pixels.Color(255, 178, 102); // “white” variable that stores the luminous color of each LED (r, g, b) in the range 0-255
uint32_t black = pixels.Color(0, 0, 0);

char ssid[] = "name";
char pass[] = "pass";
int bridge = 0;
int aCondition = 0;

BlynkTimer timer;
WidgetRTC rtc;

// Digital clock display of the time
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();

  // Send time to the App
  Blynk.virtualWrite(V1, currentTime);
  // Send date to the App
  Blynk.virtualWrite(V2, currentDate);
}

BLYNK_WRITE(V5) {                                               //Gorny czujnik dziala na pinie wirtualnym V5
  int pinValue = param.asInt();                                 // pinData variable will store value that came via Bridge
  bridge = pinValue;
  Serial.print("Bridge: ");
  Serial.print(bridge);
}

BLYNK_CONNECTED() {                                             //Start Blynk connection
  rtc.begin();
}

//Config from up to down
void dol() {
  for (int i = 320; i > 0; i--) {
    pixels.setPixelColor(i, pixels.Color(210, 170, 100));
    pixels.show();
    delay(2);
  }
  delay(20000);
  for (int i = 320; i >= 0; i--) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
    delay(2);
  }
  bridge = 0;
}

//KONFIGURACJA W GORE
void fromUpper() {
  for (int i = 0; i < 50; i++) {                                //Light the first 5 steps in a different color white
    pixels.setPixelColor(i, pixels.Color(200, 170, 80));
    pixels.show();
    delay(2);
  }
  for (int i = 50; i < NUMPIXELS; i++) {                        //Lighting of the remaining staircase
    pixels.setPixelColor(i, pixels.Color(210, 170, 100));
    pixels.show();
    delay(2);
  }
  delay(20000);
  for (int i = 0; i < NUMPIXELS; i++) {                        //Shuting down leds
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
    delay(2);
  }
}

void slideShort() {
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 16; j++) {
      pixels.setPixelColor(i + j * 20, white);
      pixels.show();
      delay(1);
    }
  }
  delay(20000);
  for (int i = 0; i < 19; i++) {
    for (int j = 0; j < 16; j++) {
      pixels.setPixelColor(i + j * 20, pixels.Color(0, 0, 0));
      pixels.show();
      delay(1);
    }
  }
}

void fromMIddle() {
  for (int i = 9; i < 21; i++) {
    for (int j = 0; j < 17; j++) {
      pixels.setPixelColor(i + j * 20, white);
      pixels.setPixelColor(j * 20 - (i + 1), white);
      pixels.show();
      delay(1);
    }
  }
  delay(20000);
  for (int i = 9; i < 19; i++) {
    for (int j = 0; j < 17; j++) {
      pixels.setPixelColor(i + j * 20, black);
      pixels.setPixelColor(j * 20 - (i + 1), black);
      pixels.show();
      delay(1);
    }
  }
}

void setup()
{
  pinMode(D1, OUTPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);

  pixels.begin();
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  setSyncInterval(60); // Sync interval in seconds (10 minutes)
  timer.setInterval(10000L, clockDisplay);                                    //Output of the current time on the console every 10 seconds
}


void loop() {
  int sunrise = int(383 + (98 * cos(((((month() - 1) * 30.5) + day()) + 8) / 58.1)));   //Sunrise time (There is special mathematical formula which calculate sunrise and sunset linked in readme)
  int sunset = int(1118 + (162 * sin(((((month() - 1) * 30.5) + day()) - 83) / 58.1))); //Sunset time
  int aTime = (hour() * 60) + minute();                                                //Actual time
  Blynk.run();
  timer.run();

  if ((aTime > 55) && (aTime < sunrise)) {                                            //Function to reduce brightness in hours 1-3
    pixels.setBrightness(64);
    //Serial.print(aTime);
  }
  if ((aTime > 200) && (aTime < sunrise)) {
    pixels.setBrightness(30);
    // Serial.print(aTime);
  }
  if ((aTime < 1430) && (aTime > sunset)) {
    pixels.setBrightness(200);
    // Serial.print(aTime);
  }
  if ((aTime > sunset) || (aTime < sunrise))
  {
    if (digitalRead(D2)) {
      switch (aCondition) {
        case 1:
          slideShort();
          aCondition++;
          break;
        case 2:
          fromUpper();
          aCondition++;
          break;
        case 3:
          fromMIddle();
          aCondition++;
          break;
        default:
          aCondition = 1;
          break;
      }
    }
    if (bridge) {
      switch (aCondition) {
        case 1:
          slideShort();
          aCondition++;
          break;
        case 2:
          dol();
          aCondition++;
          break;
        case 3:
          fromMIddle();
          aCondition++;
        default:
          aCondition = 1;
          break;
      }
    }
  }
    if ((aTime < sunset) || (aTime > sunrise)){
    pixels.fill(black, 0, 321);
    pixels.show();
  }
}
