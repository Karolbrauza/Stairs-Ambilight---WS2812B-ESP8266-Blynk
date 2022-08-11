github firs/*
  ----------------------------KOD OSWIETLENIA SCHODOW----------------------------

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

/* KONFIGURACJA FILTRA PIR
  //#define pinPIR1 D2                                             // The PIR1 sensor pin
  const int calibrationTime = 30;                                    // the time we give the sensor to calibrate (10-60 secs according to the datasheet)
  volatile int candidatePIR1;                                        // the un-filtered new status candidate of the PIR sensor
  int statusPIR1 = HIGH;                                             // the noise/spike-filtered status of the PIR sensor
  volatile boolean changePIR1 = false;                               // the indication that a potential status change has happened
  volatile long unsigned int changeTimePIR1 = 0;                     // the time stamp of the potential status change (edge timer start)
  const long unsigned int delayNoise = 1000;                          // milliseconds. Status changes shorter than this are deemed noise/spike. Test to find the best value. 100ms works for me.
  //KONIEC PIR
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
char auth[] = "FC01rJXQzlIvwn2KqyTG1BkyF0rJuuQ9";
uint32_t white = pixels.Color(255, 178, 102); // zmienna "white" przechowujaca kolor swiecenia kazdej z diod (r, g, b) w zakresie 0-255
uint32_t black = pixels.Color(0, 0, 0);

char ssid[] = "Roman Wi-Fi";
char pass[] = "rlac-yf4g-ib06";
int bridge = 0;
int warunek = 0;

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

BLYNK_CONNECTED() {                                             //Start lacznosci blynk
  rtc.begin();
}

//KONFIGURACJA W DOL
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
void gora() {
  for (int i = 0; i < 50; i++) {                                //Zapalanie pierwszych 5 schodow w innej barwie bieli
    pixels.setPixelColor(i, pixels.Color(200, 170, 80));
    pixels.show();
    delay(2);
  }
  for (int i = 50; i < NUMPIXELS; i++) {                        //Zapalenie pozostale schody
    pixels.setPixelColor(i, pixels.Color(210, 170, 100));
    pixels.show();
    delay(2);
  }
  delay(20000);
  for (int i = 0; i < NUMPIXELS; i++) {                        //Gaszenie ledow
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
    delay(2);
  }
}

void zasuwanko_short() {
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

void nodsrodkowy() {
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
  timer.setInterval(10000L, clockDisplay);                                    //Wypisanie na konsoli aktualnego czasu CO 10 SEKUND
}


void loop() {
  int sunrise = int(383 + (98 * cos(((((month() - 1) * 30.5) + day()) + 8) / 58.1)));   //Wschód słonca z wzoru
  int sunset = int(1118 + (162 * sin(((((month() - 1) * 30.5) + day()) - 83) / 58.1))); //Zachod słonca z wzoru
  int czasik = (hour() * 60) + minute();                                                //Aktualny czas
  Blynk.run();
  timer.run();

  if ((czasik > 55) && (czasik < sunrise)) {                                            //Funkcja "if" zmniejszajaca jasnosc w godzinach 1-3
    pixels.setBrightness(64);
    //Serial.print(czasik);
  }
  if ((czasik > 200) && (czasik < sunrise)) {
    pixels.setBrightness(30);
    // Serial.print(czasik);
  }
  if ((czasik < 1430) && (czasik > sunset)) {
    pixels.setBrightness(200);
    // Serial.print(czasik);
  }
  if ((czasik > sunset) || (czasik < sunrise))
  {
    if (digitalRead(D2)) {
      switch (warunek) {
        case 1:
          zasuwanko_short();
          warunek++;
          break;
        case 2:
          gora();
          warunek++;
          break;
        case 3:
          nodsrodkowy();
          warunek++;
          break;
        default:
          warunek = 1;
          break;
      }
    }
    if (bridge) {
      switch (warunek) {
        case 1:
          zasuwanko_short();
          warunek++;
          break;
        case 2:
          dol();
          warunek++;
          break;
        case 3:
          nodsrodkowy();
          warunek++;
        default:
          warunek = 1;
          break;
      }
    }
  }
    if ((czasik < sunset) || (czasik > sunrise)){
    pixels.fill(black, 0, 321);
    pixels.show();
  }
}
