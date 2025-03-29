#include <Wire.h>
#include <DFRobot_RGBLCD1602.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

Servo servoMain;
int trigpin3 = 13;
int echopin3 = 12;
int trigpin1 = 2;
int echopin1 = 3;
int trigpin2 = 5;
int echopin2 = 4;
float level1, level2, distance;
const int soilSensorPin = A0;
SoftwareSerial gsmSerial(11, 10);
SoftwareSerial gpsSerial(9, 6);
TinyGPS gps;
DFRobot_RGBLCD1602 lcd(0x27, 16, 2);
unsigned long previousMillis = 0;
const long interval = 1000;
float latitude = 29.9695, longitude = 76.8783;

void setup() {
  servoMain.attach(7);
  pinMode(trigpin1, OUTPUT);
  pinMode(echopin1, INPUT);
  pinMode(trigpin2, OUTPUT);
  pinMode(echopin2, INPUT);
  pinMode(trigpin3, OUTPUT);
  pinMode(echopin3, INPUT);
  lcd.init();
  lcd.setRGB(0, 255, 0);
  lcd.setCursor(0, 0);
  lcd.print("Hello!");
  Serial.begin(9600);
  gsmSerial.begin(9600);
  gpsSerial.begin(9600);
  delay(500);
  Serial.print("Smart Garbage Management System");
  sendSMS("System Initialized");
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    int SoilMoisture = analogRead(soilSensorPin);
    distance = getDistance(trigpin1, echopin1);
    level1 = getDistance(trigpin2, echopin2);
    level2 = getDistance(trigpin3, echopin3);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dist: ");
    lcd.print(distance);
    lcd.print(" cm");
    handleBinStatus(level1, level2);
    if (distance > 0 && distance <= 30) {
      handleTrashDetection(SoilMoisture);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No Trash");
    }
    handleWarnings();
    fetchGPSData();
  }
}

void handleBinStatus(float level1, float level2) {
  if (level1 <= 30 || level2 <= 30) {
    if (level1 <= 30 && level2 <= 30) {
      lcd.setCursor(0, 1);
      lcd.print("Both Bins Full");
      sendSMS("Both bins full. Empty bins. Location: " + getLocation());
    } else if (level1 <= 30) {
      lcd.setCursor(0, 1);
      lcd.print("Wet Bin Full");
      sendSMS("Wet bin full. Empty the bin. Location: " + getLocation());
    } else if (level2 <= 30) {
      lcd.setCursor(0, 1);
      lcd.print("Dry Bin Full");
      sendSMS("Dry bin full. Empty the bin. Location: " + getLocation());
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Take Out Trash");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Bins: Good to Go");
  }
}

void handleTrashDetection(int SoilMoisture) {
  lcd.clear();
  if (level2 > 30 && SoilMoisture < 500) {
    lcd.setCursor(0, 0);
    lcd.print("Trash Detected");
    lcd.setCursor(0, 1);
    lcd.print("Dry Garbage");
    handleDryTrash();
    checkBinStatusAfterDetection();
  } else if (level1 > 30 && SoilMoisture >= 500) {
    lcd.setCursor(0, 0);
    lcd.print("Trash Detected");
    lcd.setCursor(0, 1);
    lcd.print("Wet Garbage");
    handleWetTrash();
    checkBinStatusAfterDetection();
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Closed");
    servoMain.write(0);
  }
}

void checkBinStatusAfterDetection() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Check Bins Status");
  level1 = getDistance(trigpin2, echopin2);
  level2 = getDistance(trigpin3, echopin3);
  if (level1 <= 30) {
    lcd.setCursor(0, 1);
    lcd.print("Wet Bin Full");
  } else if (level2 <= 30) {
    lcd.setCursor(0, 1);
    lcd.print("Dry Bin Full");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Bins: Good to Go");
  }
}

void handleDryTrash() {
  for (int pos = 90; pos >= 0; pos--) {
    if (level2 > 30) {
      servoMain.write(pos);
      delay(5);
    }
  }
  servoMain.write(97);
}

void handleWetTrash() {
  for (int pos = 90; pos <= 180; pos++) {
    if (level1 > 30) {
      servoMain.write(pos);
      delay(5);
    }
  }
  servoMain.write(97);
}

void sendSMS(String message) {
  gsmSerial.println("AT+CMGF=1");
  delay(100);
  gsmSerial.println("AT+CMGS=\"+916457887550\"");
  delay(100);
  gsmSerial.println(message);
  delay(100);
  Serial.write(26);
}

void fetchGPSData() {
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    if (gps.encode(c)) {
      gps.f_get_position(&latitude, &longitude);
      Serial.print("Lat: ");
      Serial.print(latitude, 6);
      Serial.print(" Lon: ");
      Serial.println(longitude, 6);
    }
  }
}

String getLocation() {
  return String(latitude, 6) + ", " + String(longitude, 6);
}

float getDistance(int trigpinNo, int echopinNo) {
  digitalWrite(trigpinNo, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpinNo, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpinNo, LOW);
  float duration = pulseIn(echopinNo, HIGH);
  return duration * 0.0343 / 2.0;
}

void handleWarnings() {
  if ((level1 >= 30 && level1 <= 60) || (level2 >= 30 && level2 <= 60)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Warning");
    for (int i = 0; i < 3; i++) {
      tone(2, 1000, 100);
      delay(200);
    }
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Going To Full");
    tone(2, 1000);
    delay(100);
    noTone(2);
  }

  if (level1 < 30 || level2 < 30) {
    tone(2, 1000);
  } else {
    noTone(2);
  }
}
