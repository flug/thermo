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

 This example shows how value can be pushed from Arduino to
 the Blynk App.

WARNING :
For this example you'll need Adafruit DHT sensor libraries:
https://github.com/adafruit/Adafruit_Sensor
https://github.com/adafruit/DHT-sensor-library

App project setup:
Value Display widget attached to V5
Value Display widget attached to V6
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "";
int currentColor = 9999; //(Numbers correspond to chart. See below.)
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

#define DHTPIN 14          // What digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11     // DHT 11
#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;


#define PIN 4
#define NUMB_PIXEL 4
#define LED_HUMDIDITY  0 
#define LED_TEMPERATURE  1 
int cRed[3] = {255,0,0};
int cOrange[3] = {255,90,0};
int cYellow[3] = {255,255,0};
int cGreen[3] = {0,255,0};
int cBlue[3] = {0,175,255};
int cPurple[3] = {255,0,255};
int cWhite[3] = {255,255,255};



Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMB_PIXEL, PIN, NEO_GRB + NEO_KHZ800);

void  setColors(int pixelIndex, int r , int g, int b ){
        strip.setPixelColor(pixelIndex,strip.Color(r, g, b ));
      
 }

void setHumidity(int pixelIndex, int humidity)
{
   
    if (humidity <= 0 || humidity <= 17)
  {
    setColors(pixelIndex, cRed[0], cRed[1], cRed[2]);
  }
  else if (humidity <= 18 || humidity <= 34)
  {
    setColors(pixelIndex, cOrange[0], cOrange[1], cOrange[2]);
  }
  else if (humidity <= 35 || humidity <= 51)
  {
    setColors(pixelIndex, cYellow[0], cYellow[1], cYellow[2]);
  }
  else if (humidity <= 58 || humidity <= 68)
  {
    setColors(pixelIndex, cGreen[0], cGreen[1], cGreen[2]);
  }
  else if (humidity <= 69 || humidity <= 85)
  {
    setColors(pixelIndex, cBlue[0], cBlue[1], cBlue[2]);
  }
  else
  {
    //humidity <=86 && humidity <= 100 :
    setColors(pixelIndex, cPurple[0], cPurple[1], cRed[2]);
  }
}
void setTemp(int pixelIndex, int temp)
{
   
    if (temp <= -10 || temp <= 0)
  {
    setColors(pixelIndex, cWhite[0], cWhite[1], cWhite[2]);
   
  }
   else if (temp <= 0 || temp <=18)
  {
    setColors(pixelIndex, cBlue[0], cBlue[1], cBlue[2]);
  }
  else if (temp <= 18 || temp <= 21)
  {
    setColors(pixelIndex, cGreen[0], cGreen[1], cGreen[2]);
  }
  else if (temp <= 22 || temp <= 25)
  {
     setColors(pixelIndex, cYellow[0], cYellow[1], cYellow[2]);
  } else if (temp <= 26 || temp <= 28)
  {
     setColors(pixelIndex, cOrange[0], cOrange[1], cOrange[2]);
  } else if (temp <= 29 || temp <= 40)
  {
     setColors(pixelIndex, cRed[0], cRed[1], cRed[2]);
  }

 }



// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
        float h = dht.readHumidity();
        float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
        
               
        if (isnan(h) || isnan(t)) {
                Serial.println("Failed to read from DHT sensor!");
                return;
        } 
        setHumidity(LED_HUMDIDITY, round(h)); 
        setTemp(LED_TEMPERATURE, round(t)); 
        strip.show();
        // You can send any value at any time.
        // Please don't send more that 10 values per second.
        Blynk.virtualWrite(V5, h);
        Blynk.virtualWrite(V6, t);
}

void setup()
{
        // Debug console
        Serial.begin(9600);

        Blynk.begin(auth, ssid, pass);
 
        dht.begin();
        strip.begin();
        strip.show();
        // Setup a function to be called every second
        timer.setInterval(1000L, sendSensor);
        
}

void loop()
{
        strip.clear();
        Blynk.run();
        timer.run();
}
