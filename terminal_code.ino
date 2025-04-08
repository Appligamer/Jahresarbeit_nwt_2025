
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buttonArmPin = 2;
const int buttonMinusPin = 3;
const int buttonPlusPin = 4;
const int redPin = 5;
const int greenPin = 6;
const int bluePin = 7;

int alarmDistance = 50;
bool alarmArmed = false;

bool lastArmState = HIGH;
bool lastMinusState = HIGH;
bool lastPlusState = HIGH;

void setup() {
  pinMode(buttonArmPin, INPUT_PULLUP);
  pinMode(buttonMinusPin, INPUT_PULLUP);
  pinMode(buttonPlusPin, INPUT_PULLUP);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
  updateDisplay();
  updateRGB();
  sendStatus();
  sendDistance();
}

void loop() {
  handleButtons();
  delay(100);
}

void handleButtons() {
  bool armState = digitalRead(buttonArmPin);
  bool minusState = digitalRead(buttonMinusPin);
  bool plusState = digitalRead(buttonPlusPin);

  if (armState == LOW && lastArmState == HIGH) {
    alarmArmed = !alarmArmed;
    updateDisplay();
    updateRGB();
    sendStatus();
  }

  if (minusState == LOW && lastMinusState == HIGH) {
    if (alarmDistance > 10) {
      alarmDistance -= 10;
      updateDisplay();
      sendDistance();
    }
  }

  if (plusState == LOW && lastPlusState == HIGH) {
    if (alarmDistance < 200) {
      alarmDistance += 10;
      updateDisplay();
      sendDistance();
    }
  }

  lastArmState = armState;
  lastMinusState = minusState;
  lastPlusState = plusState;
}

void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Status: ");
  lcd.print(alarmArmed ? "ARMED" : "DISARMED");

  lcd.setCursor(0, 1);
  lcd.print("Dist: ");
  lcd.print(alarmDistance);
  lcd.print(" cm");
}

void updateRGB() {
  if (alarmArmed) {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
  } else {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
  }
  digitalWrite(bluePin, LOW);
}

void sendStatus() {
  Serial.print("STATUS:");
  Serial.println(alarmArmed ? "ARMED" : "DISARMED");
}

void sendDistance() {
  Serial.print("DIST:");
  Serial.println(alarmDistance);
}
