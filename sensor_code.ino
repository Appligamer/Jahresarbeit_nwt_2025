#include <SoftwareSerial.h>

SoftwareSerial terminalSerial(10, 11); // RX, TX

const int trigPin = 6;
const int echoPin = 7;
const int ledPin = 13;

int alarmState = 0;
int ae = 50;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  terminalSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // Empfange Steuerdaten
  if (terminalSerial.available()) {
    String input = terminalSerial.readStringUntil('\n');
    if (input.startsWith("S")) {
      input.remove(0, 1);
      int comma = input.indexOf(',');
      if (comma > 0) {
        alarmState = input.substring(0, comma).toInt();
        ae = input.substring(comma + 1).toInt();
      }
    }
  }

  // Entfernung messen
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distanz: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Alarm auslösen wenn aktiv & Abstand zu klein
  if (alarmState == 2 && distance <= ae) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(300);
}
