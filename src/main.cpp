#include "DHT.h"
#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27,20,4);

//====================================== Temperatureinstellungen
#define solltemp 22
#define stufe1 solltemp+1
#define stufe2 solltemp+2
#define stufe3 solltemp+3
#define stufe4 solltemp+4
#define stufe5 solltemp+5
//==============================================================

#define DHTPIN 24
#define FanPin 5
#define DHTTYPE DHT11
float temp;
String status;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
lcd.init();
lcd.backlight();
dht.begin();
TCCR1B = TCCR1B & B11111000 | B00000001;
pinMode(FanPin,OUTPUT);
}
void loop() {
delay(2000);
float temp = dht.readTemperature();
if (temp <= solltemp) { digitalWrite(FanPin,LOW); status = "AUS      "; }else
if (temp >= stufe5) { digitalWrite(FanPin,HIGH); status = "Stufe-MAX"; }else
if (temp >= stufe4) { analogWrite(FanPin,190); status = "Stufe-4  "; }else
if (temp >= stufe3) { analogWrite(FanPin,150); status = "Stufe-3  "; }else
if (temp >= stufe2) { analogWrite(FanPin,90); status = "Stufe-2  "; }else
if (temp >= stufe1) { analogWrite(FanPin,60); status = "Stufe-1  "; }else
if (temp >= solltemp) { analogWrite(FanPin,40); status = "Stufe-LOW"; }
lcd.setCursor(0,0);
lcd.print("L\365ftersteuerung");
lcd.setCursor(0,1);
lcd.print("Aktuell: ");
lcd.print(temp);
lcd.print("\337C");
lcd.setCursor(0,2);
lcd.print("Soll: ");
lcd.print(solltemp);
lcd.print("\337C");
lcd.setCursor(0,3);
lcd.print("Status: ");
lcd.print(status);
delay(1000);
}
