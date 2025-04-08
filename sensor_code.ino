
const int trigPin = 9;
const int echoPin = 10;
const int alarmPin = 6;

int alarmDistance = 50; // Standard-Entfernung in cm
bool alarmArmed = false;

String input = "";

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(alarmPin, OUTPUT);
  
  Serial.begin(9600); // Serielle Verbindung zum Terminal
}

void loop() {
  readSerialInput();
  
  if (alarmArmed) {
    long distance = measureDistance();
    
    if (distance > 0 && distance <= alarmDistance) {
      digitalWrite(alarmPin, HIGH); // Alarm auslösen
    } else {
      digitalWrite(alarmPin, LOW); // Kein Alarm
    }
  } else {
    digitalWrite(alarmPin, LOW); // Immer aus
  }

  delay(200);
}

long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout bei 30ms
  if (duration == 0) return -1; // Kein Echo

  long distance = duration * 0.034 / 2; // in cm
  return distance;
}

void readSerialInput() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      processMessage(input);
      input = "";
    } else {
      input += c;
    }
  }
}

void processMessage(String msg) {
  msg.trim();
  
  if (msg.startsWith("DIST:")) {
    int dist = msg.substring(5).toInt();
    if (dist >= 10 && dist <= 200) {
      alarmDistance = dist;
    }
  } else if (msg.startsWith("STATUS:")) {
    String status = msg.substring(7);
    alarmArmed = (status == "ARMED");
  }
}
