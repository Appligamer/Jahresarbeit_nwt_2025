#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial sensorSerial(10, 11); // RX, TX

const int buttonPin = 2;
const int potPin = A0;
int alarmState = 0; // 0 = Aus, 1 = Entschärft, 2 = Scharf
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  sensorSerial.begin(9600);
  Serial.begin(9600);
  updateLCD();
}

void loop() {
  // Knopf einlesen mit Entprellen
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && lastButtonState == HIGH) {
      alarmState = (alarmState + 1) % 3;
      updateLCD();
    }
  }
  lastButtonState = reading;

  // AE-Wert vom Potentiometer
  int potValue = analogRead(potPin);       // 0 bis 1023
  int ae = map(potValue, 0, 1023, 1, 100);  // 1 bis 100 cm

  // An Sensor senden
  sensorSerial.print("S");
  sensorSerial.print(alarmState);
  sensorSerial.print(",");
  sensorSerial.println(ae);

  // Status anzeigen
  lcd.setCursor(0, 1);
  lcd.print("AE: ");
  lcd.print(ae);
  lcd.print(" cm   ");

  delay(500);
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (alarmState) {
    case 0: lcd.print("Status: AUS     "); break;
    case 1: lcd.print("Status: ENTSCH. "); break;
    case 2: lcd.print("Status: SCHARF  "); break;
  }
}
