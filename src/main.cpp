#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h> // Library für Rotary Encoder

// Pin-Definitionen
#define DHTPIN 4  // Pin, an dem der DHT11 angeschlossen ist
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
float targetTemperature = 25.0; // Solltemperatur in °C
float temperatureSpread = 5.0;  // Spreizung in °C (Temperaturunterschied für Vollast)
float relaySpread = 0.0;       // Spreizung für das Relais in °C (auf 0 gesetzt)

long lastPosition = -999; // Letzte bekannte Position des Encoders
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // Debouncing-Zeit für den Button

void setup() {
  pinMode(FAN_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(ENCODER_BTN, INPUT_PULLUP);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);

  // Initialisiere PWM-Wert auf 0 (Lüfter aus beim Start)
  analogWrite(FAN_PIN, 0);
}

void loop() {
  // Aktuelle Temperatur vom DHT11 lesen
  float currentTemperature = dht.readTemperature();

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
    float pwmRatio = constrain((temperatureDifference / temperatureSpread), 0.0, 1.0);
    pwmValue = 255 - (int)(pwmRatio * 255); // Invertiertes PWM-Signal

    // Mindest-PWM-Schwelle berücksichtigen
    if (pwmValue > 0 && pwmValue < 77) { // 77 entspricht ca. 30% von 255
      pwmValue = 77; // Lüfter startet bei 30%
    }
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
  lcd.print(100 - map(pwmValue, 0, 255, 0, 100)); // Invertierte Anzeige
  lcd.print(" % ");

  lcd.setCursor(0, 3);
  lcd.print("Relais: ");
  lcd.print(currentTemperature < targetTemperature - relaySpread ? "AUS" : "EIN");

  // Kurze Pause
  delay(100);
}
