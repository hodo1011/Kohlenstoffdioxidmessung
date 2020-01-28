/*Hodo1011  Dieser Code liest den MQ9Sensor aus, löst zum richtigen Zeitpunkt einen Alarm aus und
gibt die Werte auf dem Handy aus
-zur Messung habe ich im Programmteil 2 eines Ausgangsmesswert (in frischer Luft) erstellt und 
die realen Messwerte mit der Ausgangsmessung verglichen*/





//erster Programmteil

void loop() { 
int Analog_channel_pin =32;
sensor = analogRead(Analog_channel_pin);


//Average 
  
   for(int x = 0 ; x < 1000 ; x++) 

   
 { 
 voltage_value = ((sensor) * 5.0 ) / (4095);
 sensorValue = sensorValue + voltage_value;  
 } 
 sensorValue1 = sensorValue/1000; 




 delay(250); 
}
////////////////////////////////////////////////////////////////////////////////////////////////







//zweiter Programmteil


#include<cmath>
#include <SPI.h>
#include "SSD1306.h" 
 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BluetoothSerial.h"
#include <Arduino.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



#include <BluetoothSerial.h>
BluetoothSerial SerialBT;
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

int led=23;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

pinMode(led,OUTPUT);
 SerialBT.begin("MQ9"); //Bluetooth device name
  SerialBT.begin("MQ9"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  

int analog;
double y;
float sensor_volt; 
float Verhaltnis; 
float Gaskonzentrationluft; 
float reineluftwert = 1.3; 
int analogeinlesen = analogRead(32); 
double COppm;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);

   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
   }

sensor_volt = ((float)analogeinlesen / 4095) * 5.0; 
Gaskonzentrationluft = (5.0 - sensor_volt) / sensor_volt; 
Verhaltnis = Gaskonzentrationluft / reineluftwert; // ratio = RS/R0 
y=Verhaltnis; //Verhaltnis = Gaskonzentrationluft / reineluftwert; 

COppm= 678.4805526/pow(y, 1.941154808); 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
if(COppm<=800)
{
display.clearDisplay();
display.setCursor(0,0);
display.print("Ihr Messergebnisse: ");
display.setTextColor(WHITE);
display.setCursor(0,30);
display.print("CO: ");

display.print(COppm);
display.setCursor(50,30);
display.print("ppm");
SerialBT.print("Ihr Messergebnis von Kohlenstoffmonoxid:");
SerialBT.println(COppm);
display.display();
digitalWrite(led,LOW);
delay(400);  
}
 if( COppm>800)
{
Serial.println("Alarm, Grenze von 800 ppm ist erreicht!!!");
Serial.println("Bitte Fenster oeffnen!!! ");
digitalWrite(led,HIGH);
SerialBT.println("Grenze von 800ppm erreicht!");
display.clearDisplay();
display.setTextColor(WHITE);
display.setCursor(0,0);
display.setTextSize(1);

display.print("800 ppm erreicht!!!");
display.setCursor(10,30);
display.print("Fenster oeffnen!!");
display.display();
delay(400);

}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
