#define BLYNK_TEMPLATE_ID "TMPL3RS_hHAPM"
#define BLYNK_TEMPLATE_NAME "world"
#define BLYNK_AUTH_TOKEN "62LAfNKzC5GcoVSRd-uFTgBfM08GdsgH"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// WiFi credentials
char ssid[] = "BSFC_COWORKER";
char pass[] = "sai@12123";

// Motor driver pins
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33
#define ENA 14
#define ENB 32

// LED pins for both red lights
#define RED_LED_1 19 // First red LED
#define RED_LED_2 18 // Second red LED

// Variables for blinking LEDs
unsigned long previousMillis = 0;
const long interval = 200; // Blink interval in milliseconds (e.g., 200ms for 5 blinks/second)
int ledState = LOW;

// === Motor Control ===
void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 200); analogWrite(ENB, 200);
}

void backward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 200); analogWrite(ENB, 200);
}

void left() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 200); analogWrite(ENB, 200);
}

void right() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 200); analogWrite(ENB, 200);
}

// === Blynk Virtual Pin Functions ===
BLYNK_WRITE(V0) { (param.asInt()) ? forward() : stopMotors(); }
BLYNK_WRITE(V1) { (param.asInt()) ? backward() : stopMotors(); }
BLYNK_WRITE(V2) { (param.asInt()) ? left() : stopMotors(); }
BLYNK_WRITE(V3) { (param.asInt()) ? right() : stopMotors(); }
BLYNK_WRITE(V4) { stopMotors(); }

void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  // LED pins for both red lights
  pinMode(RED_LED_1, OUTPUT);
  pinMode(RED_LED_2, OUTPUT);

  stopMotors(); // Ensure motors are OFF initially

  // Connect to WiFi + Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  // Blinking both Red LEDs in sync
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = (ledState == LOW) ? HIGH : LOW; // Toggle the state
    digitalWrite(RED_LED_1, ledState);         // Apply state to first LED
    digitalWrite(RED_LED_2, ledState);         // Apply state to second LED
  }
}