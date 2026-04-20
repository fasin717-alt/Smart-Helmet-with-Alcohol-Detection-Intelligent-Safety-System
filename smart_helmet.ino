#include <SoftwareSerial.h>

// Pin Definitions
#define ALCOHOL_SENSOR A0
#define HELMET_SENSOR 2
#define RELAY_PIN 3
#define BUZZER 4
#define LED 5

// GSM (SIM800L)
SoftwareSerial gsm(7, 8); // RX, TX

int alcoholThreshold = 400;  // Adjust after testing
bool helmetStatus = false;

void setup() {
  pinMode(ALCOHOL_SENSOR, INPUT);
  pinMode(HELMET_SENSOR, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
  gsm.begin(9600);

  digitalWrite(RELAY_PIN, LOW); // Bike OFF initially
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED, LOW);

  Serial.println("Smart Helmet System Started");
}

void loop() {
  int alcoholValue = analogRead(ALCOHOL_SENSOR);
  helmetStatus = digitalRead(HELMET_SENSOR);

  Serial.print("Alcohol Value: ");
  Serial.println(alcoholValue);

  Serial.print("Helmet Status: ");
  Serial.println(helmetStatus);

  // Condition Check
  if (helmetStatus == HIGH && alcoholValue < alcoholThreshold) {
    // SAFE CONDITION
    digitalWrite(RELAY_PIN, HIGH); // Bike ON
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, LOW);
    Serial.println("SAFE: Bike Started");
  } 
  else {
    // UNSAFE CONDITION
    digitalWrite(RELAY_PIN, LOW); // Bike OFF
    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED, HIGH);
    Serial.println("UNSAFE: Bike Stopped");
  }

  // Simulated Accident Detection (Replace with MPU6050 later)
  if (detectAccident()) {
    Serial.println("Accident Detected!");
    sendSMS();
    delay(10000); // Avoid repeated messages
  }

  delay(1000);
}

// Dummy Accident Function (Replace with MPU6050 logic)
bool detectAccident() {
  // For testing: trigger accident if alcohol value very high
  if (analogRead(ALCOHOL_SENSOR) > 800) {
    return true;
  }
  return false;
}

// GSM SMS Function
void sendSMS() {
  gsm.println("AT+CMGF=1"); // Text mode
  delay(1000);
  
  gsm.println("AT+CMGS=\"+91XXXXXXXXXX\""); // Replace with phone number
  delay(1000);
  
  gsm.println("Accident detected! Please help. Location: Not available");
  delay(1000);
  
  gsm.write(26); // CTRL+Z
  delay(3000);

  Serial.println("SMS Sent");
