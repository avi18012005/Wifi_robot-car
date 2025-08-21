# Arduino UNO WiFi Car with Blynk App - Complete Guide

## Project Overview
This project creates a WiFi-controlled car using Arduino UNO and the Blynk mobile app. The car can be controlled remotely through a smartphone using the Blynk interface.

## Required Components

### Core Components
- **1x Arduino UNO R3**
- **1x ESP8266 WiFi Module (ESP-01)**
- **1x L298N Motor Driver Module**
- **2x DC Motors (3-6V)**
- **1x Car Chassis Kit** (includes wheels, chassis, and motor mounts)
- **1x 18650 Battery Holder** (2-cell)
- **2x 18650 Li-ion Batteries** (3.7V each)
- **1x Battery Charger** (for 18650 batteries)

### Wiring Components
- **Jumper Wires** (Male-to-Male, Male-to-Female)
- **Breadboard** (half-size)
- **Resistors**:
  - 2x 1kΩ resistors
  - 2x 2.2kΩ resistors
- **Capacitors** (optional for power stability):
  - 2x 100µF electrolytic capacitors

### Tools Required
- Soldering iron (if needed for connections)
- Wire strippers
- Small screwdriver set
- Multimeter (for testing)

## Pin Configuration

### Arduino UNO to ESP8266 (WiFi Module)
| Arduino UNO | ESP8266 | Description |
|-------------|---------|-------------|
| 3.3V | VCC | Power (3.3V ONLY) |
| GND | GND | Ground |
| TX (Pin 1) | RX | Data Receive |
| RX (Pin 0) | TX | Data Transmit |
| 3.3V | CH_PD | Enable pin |

**Important Notes:**
- ESP8266 requires 3.3V logic level
- Use voltage divider for Arduino TX to ESP8266 RX
- Arduino RX can connect directly to ESP8266 TX

### Arduino UNO to L298N Motor Driver
| Arduino UNO | L298N | Function |
|-------------|--------|----------|
| 5V | +5V | Power for logic |
| GND | GND | Ground |
| Pin 8 | IN1 | Motor A Direction 1 |
| Pin 9 | IN2 | Motor A Direction 2 |
| Pin 10 | IN3 | Motor B Direction 1 |
| Pin 11 | IN4 | Motor B Direction 2 |
| VIN | +12V | Motor Power (from battery) |

### L298N to Motors
| L298N | Component |
|--------|-----------|
| OUT1 | Motor A Terminal 1 |
| OUT2 | Motor A Terminal 2 |
| OUT3 | Motor B Terminal 1 |
| OUT4 | Motor B Terminal 2 |

### Power Connections
| Component | Power Source | Voltage |
|-----------|---------------|---------|
| Arduino UNO | Battery via VIN | 7-12V |
| L298N Motors | Battery direct | 7.4V (2x18650) |
| ESP8266 | Arduino 3.3V | 3.3V |
| L298N Logic | Arduino 5V | 5V |

## Blynk App Configuration

### Blynk App Setup
1. **Download Blynk App** from Google Play Store or Apple App Store
2. **Create Account** or log in to existing account
3. **Create New Project**:
   - Project Name: "WiFi Car"
   - Device: Arduino UNO
   - Connection Type: WiFi
   - Auth Token: Save this token (you'll need it in code)

### Blynk Widget Configuration
Add the following widgets to your project:

1. **Joystick Widget**:
   - Name: "Controller"
   - Output: Virtual Pin V1
   - Mode: Split
   - Merge: OFF

2. **Button Widgets** (optional):
   - Forward Button: Virtual Pin V2
   - Backward Button: Virtual Pin V3
   - Left Button: Virtual Pin V4
   - Right Button: Virtual Pin V5

## Arduino Code

### Required Libraries
Install these libraries in Arduino IDE:
- **Blynk Library** (by Volodymyr Shymanskyy)
- **ESP8266 Library** (by Ivan Grokhotkov)

### Code Structure
```cpp
// Include necessary libraries
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// WiFi credentials
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// Pin definitions
#define ENA 5    // Enable pin for Motor A (PWM)
#define ENB 6    // Enable pin for Motor B (PWM)
#define IN1 8    // Control pin 1 for Motor A
#define IN2 9    // Control pin 2 for Motor A
#define IN3 10   // Control pin 1 for Motor B
#define IN4 11   // Control pin 2 for Motor B

// ESP8266 pins
#define ESP8266_BAUD 9600
ESP8266 wifi(&Serial);

void setup() {
  Serial.begin(ESP8266_BAUD);
  Blynk.begin(auth, wifi, ssid, pass);
  
  // Set motor pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  // Initialize motors to stopped
  stopMotors();
}

void loop() {
  Blynk.run();
}

// Motor control functions
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// Blynk joystick handler
BLYNK_WRITE(V1) {
  int x = param[0].asInt();
  int y = param[1].asInt();
  
  if (y > 200) {
    moveForward();
  } else if (y < -200) {
    moveBackward();
  } else if (x > 200) {
    turnRight();
  } else if (x < -200) {
    turnLeft();
  } else {
    stopMotors();
  }
}
```

## Assembly Instructions

### Step 1: Prepare the Chassis
1. Attach the DC motors to the chassis using the provided brackets
2. Mount the wheels on the motor shafts
3. Attach the caster wheel at the front/back for balance

### Step 2: Wire the Motors
1. Connect the two left motors together (parallel connection)
2. Connect the two right motors together (parallel connection)
3. Connect these to the L298N motor driver outputs

### Step 3: Install the Electronics
1. Mount the Arduino UNO on the chassis using screws or double-sided tape
2. Mount the L298N motor driver nearby
3. Mount the ESP8266 module (use a small breadboard if needed)

### Step 4: Power Connections
1. Connect the battery holder to the L298N power input
2. Connect L298N ground to Arduino ground
3. Connect L298N 5V to Arduino VIN
4. Add power switch between battery and circuit

### Step 5: Signal Connections
1. Connect Arduino pins to L298N control pins as per pin configuration
2. Connect ESP8266 to Arduino as per pin configuration
3. Double-check all connections with multimeter

## Testing Procedure

### Initial Testing
1. **Power Test**: Check all voltages with multimeter
2. **Motor Test**: Test motors individually using simple Arduino sketch
3. **WiFi Test**: Test ESP8266 connection with WiFi scan sketch
4. **Blynk Test**: Upload final code and test with Blynk app

### Troubleshooting Common Issues

**Problem**: Motors not turning
- Check power connections
- Verify L298N power LED is on
- Check motor driver input voltage

**Problem**: WiFi not connecting
- Verify ESP8266 connections
- Check WiFi credentials
- Ensure 3.3V power to ESP8266

**Problem**: Car moves in wrong direction
- Swap motor connections on L298N outputs
- Adjust code logic for direction

## Safety Precautions
- Always disconnect power before making changes
- Use proper battery handling procedures
- Don't exceed voltage ratings
- Keep electronics away from moisture
- Use proper ventilation when soldering

## Project Extensions
- Add ultrasonic sensor for obstacle avoidance
- Add LED headlights
- Add speed control via PWM
- Add camera module for FPV
- Add Bluetooth as backup control

## Bill of Materials (Estimated Cost)
- Arduino UNO: $10-15
- ESP8266 Module: $3-5
- L298N Driver: $2-4
- DC Motors: $5-10
- Chassis Kit: $10-15
- Batteries + Holder: $8-12
- Jumper Wires: $3-5
- **Total: $41-66**

## Useful Links
- [Blynk Documentation](https://docs.blynk.io/)
- [Arduino Reference](https://www.arduino.cc/reference/en/)
- [L298N Datasheet](https://www.st.com/resource/en/datasheet/l298.pdf)
- [ESP8266 AT Commands](https://room-15.github.io/blog/2015/03/26/esp8266-at-command-reference/)

## Final Notes
- Test each component individually before full assembly
- Keep wires organized and labeled
- Take photos during assembly for reference
- Start with basic movement, then add features
- Join Arduino and Blynk communities for support
