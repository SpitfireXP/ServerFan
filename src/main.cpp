<<<<<<< HEAD
#include "DHT.h"
#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27,20,4);
//#include <CTRL.h>
#include <CtrlEnc.h>

//====================================== Temperatureinstellungen
// #define solltemp 24
int solltemp;
#define stufe1 solltemp+1
#define stufe2 solltemp+2
#define stufe3 solltemp+3
#define stufe4 solltemp+4
#define stufe5 solltemp+5
//==============================================================


void onTurnleft() {
    solltemp = solltemp - 1;
}

void onTurnRight() {
    solltemp = solltemp + 1;
}


#define DHTPIN 4
#define FanPin 5
#define RelayPin 6
=======
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h> // Library für Rotary Encoder

// Pin-Definitionen
#define DHTPIN 4  // Pin, an dem der DHT11 angeschlossen ist
>>>>>>> 2bba967abfdb97a3cd5359b0e9a1f1159ceea956
#define DHTTYPE DHT11
#define FAN_PIN 5  // PWM-Pin für den Lüfter
#define RELAY_PIN 6 // Pin für das Relais
#define ENCODER_CLK 2  // Pin für den Encoder-CLK
#define ENCODER_DT 7   // Pin für den Encoder-DT
#define ENCODER_BTN 8  // Pin für den Encoder-Button

// DHT-Sensor initialisieren
DHT dht(DHTPIN, DHTTYPE);

// LCD initialisieren (I2C-Adresse 0x27, 20x4 Display)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Rotary Encoder initialisieren
Encoder myEnc(ENCODER_CLK, ENCODER_DT);

// Globale Variablen
float targetTemperature = 22.0; // Solltemperatur in °C
float temperatureSpread = 5.0;  // Spreizung in °C (Temperaturunterschied für Vollast)
float relaySpread = 0.5;       // Spreizung für das Relais in °C

long lastPosition = -999; // Letzte bekannte Position des Encoders
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // Debouncing-Zeit für den Button

void setup() {
<<<<<<< HEAD
lcd.init();
lcd.backlight();
dht.begin();
//TCCR1B = TCCR1B & B11111000 | B00000001;
pinMode(FanPin,OUTPUT);
pinMode(RelayPin,OUTPUT);
solltemp = 21;
=======
  pinMode(FAN_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(ENCODER_BTN, INPUT_PULLUP);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);
>>>>>>> 2bba967abfdb97a3cd5359b0e9a1f1159ceea956
}

void loop() {
<<<<<<< HEAD
  encoder.process();
delay(2000);
float temp = dht.readTemperature();
if (temp <= solltemp) { digitalWrite(RelayPin,LOW); status = "AUS      "; }else
    { digitalWrite(RelayPin,HIGH);}
=======
  // Aktuelle Temperatur vom DHT11 lesen
  float currentTemperature = dht.readTemperature();
>>>>>>> 2bba967abfdb97a3cd5359b0e9a1f1159ceea956

  // Sicherstellen, dass der Messwert gültig ist
  if (isnan(currentTemperature)) {
    Serial.println("Fehler beim Lesen des Temperatursensors!");
    lcd.setCursor(0, 0);
    lcd.print("Sensorfehler!");
    delay(1000);
    return;
  }

  // Encoder-Eingabe verarbeiten
  long newPosition = myEnc.read() / 2; // Encoder-Werte halbieren, um Schritte zu normalisieren
  if (newPosition != lastPosition) {
    targetTemperature += (newPosition - lastPosition) * 0.5; // Schrittweise Änderung um 0.5°C
    targetTemperature = constrain(targetTemperature, 10.0, 30.0); // Begrenzung der Solltemperatur
    lastPosition = newPosition;
  }

  // Button-Eingabe verarbeiten
  int buttonState = digitalRead(ENCODER_BTN);
  if (buttonState == LOW && (millis() - lastDebounceTime) > debounceDelay) {
    targetTemperature = 20.0; // Solltemperatur zurücksetzen
    lastDebounceTime = millis();
  }

  // Differenz zwischen aktueller Temperatur und Solltemperatur
  float temperatureDifference = currentTemperature - targetTemperature;

  // PWM-Signal berechnen (1-100% Bereich)
  int pwmValue = 0;
  if (temperatureDifference > 0) {
    pwmValue = map(constrain(temperatureDifference, 0, temperatureSpread), 0, temperatureSpread, 0, 255);
  }

  // PWM-Wert an Lüfter ausgeben
  analogWrite(FAN_PIN, pwmValue);

  // Relaissteuerung basierend auf Temperatur
  if (currentTemperature < targetTemperature - relaySpread) {
    digitalWrite(RELAY_PIN, LOW); // Relais einschalten (invertiert)
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Relais ausschalten (invertiert)
  }

  // Debug-Ausgabe
  Serial.print("Aktuelle Temperatur: ");
  Serial.print(currentTemperature);
  Serial.print(" °C, PWM-Wert: ");
  Serial.print(pwmValue);
  Serial.print(", Relais: ");
  Serial.println(currentTemperature < targetTemperature - relaySpread ? "AUS" : "EIN");

  // LCD-Ausgabe
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(currentTemperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Soll: ");
  lcd.print(targetTemperature);
  lcd.print(" C");

  lcd.setCursor(0, 2);
  lcd.print("PWM: ");
  lcd.print(map(pwmValue, 0, 255, 0, 100));
  lcd.print(" % ");

  lcd.setCursor(0, 3);
  lcd.print("Relais: ");
  lcd.print(currentTemperature < targetTemperature - relaySpread ? "AUS" : "EIN");

  // Kurze Pause
  delay(100);
}
