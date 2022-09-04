# Stairs Ambilight - WS2812B ESP8266 HC-SR501 Blynk 

An IoT project whose task is to illuminate stairs with different patterns after movements have been detected at certain times of the day.

At the height of the first and last step, there is mounted PIR sensor ( HC-SR501) connected to ESP8266 (Wemos D1 mini board) . 
Connection between upper and lower ESP is provided by Blynk platform which delivery bridge function. Upper station is only to recieve data from PIR sensor and transfer it to lower ESP which is main processor of this installation. In lower ESP is code to steering leds. Reason why that? Because adressable leds like WS2812B have only one direction data line and in my case they are connectet from bottom to up.

There is examle of daily working:
<p align="center">
<img src=https://user-images.githubusercontent.com/91514892/188322300-5b4ed05d-00a8-409a-ad5d-fe58fa05102a.gif>
</p>
