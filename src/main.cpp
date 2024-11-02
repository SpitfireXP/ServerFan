#include "DHT.h"
#include "LiquidCrystal_I2C.h"
//#include <CTRL.h>
#include <CtrlEnc.h>



LiquidCrystal_I2C lcd(0x27,20,4);

//====================================== Temperatureinstellungen
#define solltemp 24
#define stufe1 solltemp+1
#define stufe2 solltemp+2
#define stufe3 solltemp+3
#define stufe4 solltemp+4
#define stufe5 solltemp+5
//==============================================================

#define DHTPIN 4
#define FanPin 5
#define RelayPin 6
#define DHTTYPE DHT11
float temp;
String status;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
lcd.init();
lcd.backlight();
dht.begin();
//TCCR1B = TCCR1B & B11111000 | B00000001;
pinMode(FanPin,OUTPUT);
pinMode(RelayPin,OUTPUT);
}
void loop() {
delay(2000);
float temp = dht.readTemperature();
if (temp <= solltemp) { digitalWrite(RelayPin,LOW); status = "AUS      "; }else
    { digitalWrite(RelayPin,HIGH);}

if (temp >= stufe5) { digitalWrite(FanPin,LOW); status = "Stufe-MAX"; }else
if (temp >= stufe4) { analogWrite(FanPin,40); status = "Stufe-4  "; }else
if (temp >= stufe3) { analogWrite(FanPin,60); status = "Stufe-3  "; }else
if (temp >= stufe2) { analogWrite(FanPin,90); status = "Stufe-2  "; }else
if (temp >= stufe1) { analogWrite(FanPin,150); status = "Stufe-1  "; }else
if (temp >= solltemp) { analogWrite(FanPin,220); status = "Stufe-LOW"; }

lcd.setCursor(0,0);
lcd.print("L\365ftersteuerung");
lcd.setCursor(0,1);
lcd.print("Aktuell:  ");
lcd.print(temp);
lcd.print("\337C");
lcd.setCursor(0,2);
lcd.print("Solltemp: ");
lcd.print(solltemp);
lcd.print("\337C");
lcd.setCursor(0,3);
lcd.print("Status:   ");
lcd.print(status);
delay(1000);
}
