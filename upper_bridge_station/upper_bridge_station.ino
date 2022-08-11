/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  Control another device using Bridge widget!

  Bridge is initialized with the token of any (Blynk-enabled) device.
  After that, use the familiar functions to control it:
    bridge.digitalWrite(8, HIGH)
    bridge.digitalWrite("A0", LOW) // <- target needs to support "Named pins"
    bridge.analogWrite(3, 123)
    bridge.virtualWrite(V1, "hello")
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "token";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "name";
char pass[] = "pass";

// Bridge widget on virtual pin 1
WidgetBridge bridge1(V1);

// Timer for blynking
BlynkTimer timer;

static bool value = true;
void blynkAnotherDevice() // Here we will send HIGH or LOW once per second
{
    bridge1.virtualWrite(V5, 0); // Sends 1 value to BLYNK_WRITE(V5) handler on receiving side.
  // Send value to another device
    if(digitalRead(D2)==HIGH){
    //digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage HIGH
    bridge1.digitalWrite(D3, HIGH);  // Digital Pin 9 on the second board will be set HIGH
    bridge1.virtualWrite(V5, 1); // Sends 1 value to BLYNK_WRITE(V5) handler on receiving side.
    //delay(500);
    }else{
    bridge1.digitalWrite(D3, LOW); // Digital Pin 9 on the second board will be set LOW
   // digitalWrite(LED_BUILTIN, HIGH );   // Turn the LED on (Note that LOW is the voltage level
    bridge1.virtualWrite(V5, 0); // Sends 1 value to BLYNK_WRITE(V5) handler on receiving side.
  }
}

BLYNK_CONNECTED() 
{
  bridge1.setAuthToken("FC01rJXQzlIvwn2KqyTG1BkyF0rJuuQ9"); // Place the AuthToken of the second hardware here
}

void setup()
{
  pinMode(D2, INPUT);
 // pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(500L, blynkAnotherDevice);
}

void loop()
{
  Blynk.run();
  timer.run();
  
}
