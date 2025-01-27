#include <Adafruit_NeoPixel.h>

// Global variables for pitch and roll angles
int pitch = 0;
int roll = 0;

// Pin assignments for LED strips and fault LED
const int posPitchPin = 46;
const int negPitchPin = 9;
const int posRollPin = 10;
const int negRollPin = 11;
const int fault_led = 3;

// Maximum number of LEDs for pitch and roll LED strips
const int maxPitchLeds = 30;
const int maxRollLeds = 60;

// Pitch angle thresholds and LED zone definitions
const int pitchGreenMax = 5;     // Green zone max angle
const int pitchYellowMax = 8;    // Yellow zone max angle
const int pitchMax = 28;         // Absolute max pitch angle
const int pitchGreenLeds = 8;    // LEDs in green zone
const int pitchYellowLeds = 15;  // LEDs in yellow zone

// Roll angle thresholds and LED zone definitions
const int rollGreenMax = 8;     // Green zone max angle
const int rollYellowMax = 16;   // Yellow zone max angle
const int rollMax = 58;         // Absolute max roll angle
const int rollGreenLeds = 20;   // LEDs in green zone
const int rollYellowLeds = 40;  // LEDs in yellow zone

// Variables to store calculated LED values for pitch and roll
int posPitchLeds = 0;
int negPitchLeds = 0;
int posRollLeds = 0;
int negRollLeds = 0;

// Adafruit NeoPixel LED strips
Adafruit_NeoPixel posPitchStrip = Adafruit_NeoPixel(maxPitchLeds, posPitchPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel negPitchStrip = Adafruit_NeoPixel(maxPitchLeds, negPitchPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel posRollStrip = Adafruit_NeoPixel(maxRollLeds, posRollPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel negRollStrip = Adafruit_NeoPixel(maxRollLeds, negRollPin, NEO_GRB + NEO_KHZ800);

uint32_t red = Adafruit_NeoPixel::Color(255, 0, 0);        // Red
uint32_t green = Adafruit_NeoPixel::Color(0, 255, 0);      // Green
uint32_t white = Adafruit_NeoPixel::Color(255, 255, 255);  // White
uint32_t yellow = Adafruit_NeoPixel::Color(200, 150, 0);   // Yellow
uint32_t all_off = Adafruit_NeoPixel::Color(0, 0, 0);      // All Off

// Data buffer for serial communication
const int bufferSize = 64;
byte receivedData[bufferSize];
int bytesReceived = 0;

// Helper functions for LED strip operations
void yellow_fill(Adafruit_NeoPixel &led_strip, int endIndex);
void red_fill(Adafruit_NeoPixel &led_strip, int endIndex);
void clear_leds(Adafruit_NeoPixel &led_strip, int leds);
void glowLed(Adafruit_NeoPixel &led_strip, int leds, String direction);
void initLedStrip(Adafruit_NeoPixel &strip, int maxLeds);

// Helper functions for data processing
int mapPitch(int pitch);
int mapRoll(int roll);
void printReceivedData(int startIndex);
void shiftData(int startIdx, int numBytes);
void convertBytesToNumbers(int startIndex);

// Setup function
void setup() {
  pinMode(posPitchPin, OUTPUT);
  pinMode(negPitchPin, OUTPUT);
  pinMode(posRollPin, OUTPUT);
  pinMode(negRollPin, OUTPUT);
  pinMode(fault_led, OUTPUT);

  // Initialize LED strips
  initLedStrip(posPitchStrip, maxPitchLeds);
  initLedStrip(negPitchStrip, maxPitchLeds);
  initLedStrip(posRollStrip, maxRollLeds);
  initLedStrip(negRollStrip, maxRollLeds);

  Serial.begin(115200);
  Serial1.begin(115200);
}

// Main loop function
void loop() {
  digitalWrite(fault_led, LOW);

  if (Serial.available() > 0) {
    receivedData[bytesReceived] = Serial.read();
    bytesReceived++;

    if (bytesReceived >= 2) {
      for (int i = 0; i < bytesReceived - 1; i++) {
        if (receivedData[i] == 0x5A && receivedData[i + 1] == 0xA5) {
          if (bytesReceived - i >= 12) {
            convertBytesToNumbers(i);
            shiftData(i + 2, bytesReceived - (i + 2));
            bytesReceived -= (i + 2);
          }
          break;
        }
      }
    }

    if (bytesReceived >= bufferSize) {
      bytesReceived = 0;
    }
  } else {
    digitalWrite(fault_led, HIGH);
    Serial1.println("no byte recevied");
    clear_all();
  }
}

// Add function implementations below

// Fill LED strip with yellow color up to a specific index
void yellow_fill(Adafruit_NeoPixel &led_strip, int endIndex) {
  for (int i = 0; i <= endIndex; i++) {
    led_strip.setPixelColor(i, yellow);
  }
}

// Fill LED strip with red color up to a specific index
void red_fill(Adafruit_NeoPixel &led_strip, int endIndex) {
  for (int i = 0; i <= endIndex; i++) {
    led_strip.setPixelColor(i, red);
  }
}

// Clear all LEDs on a strip
void clear_leds(Adafruit_NeoPixel &led_strip, int leds) {
  for (int i = 0; i <= leds; i++) {
    led_strip.setPixelColor(i, all_off);  // Turn off LEDs
  }
  led_strip.show();
}

void glowLed(Adafruit_NeoPixel &led_strip, int leds, String direction) {
  int greenZone = 0;
  int yellowZone = 0;

  if (direction == "pitch") {
    greenZone = pitchGreenLeds;
    yellowZone = pitchYellowLeds;
  } else if (direction == "roll") {
    greenZone = rollGreenLeds;
    yellowZone = rollYellowLeds;
  }

  for (int i = 0; i <= leds; i++) {
    if (i <= greenZone) {
      led_strip.setPixelColor(i, green);  // Green
    } else if (i <= yellowZone) {
      yellow_fill(led_strip, greenZone);
      led_strip.setPixelColor(i, yellow);  // Yellow
    } else {
      red_fill(led_strip, yellowZone);
      led_strip.setPixelColor(i, red);  // Red
    }
  }
  led_strip.show();
}

// Map pitch value to corresponding LED count
int mapPitch(int pitch) {
  if (pitch <= pitchGreenMax) {
    return map(pitch, 0, pitchGreenMax, 0, pitchGreenLeds);
  } else if (pitch <= pitchYellowMax) {
    return map(pitch, pitchGreenMax, pitchYellowMax, pitchGreenLeds, pitchYellowLeds);
  } else {
    return map(pitch, pitchYellowMax, pitchMax, pitchYellowLeds, pitchMax);
  }
}

// Map roll value to corresponding LED count
int mapRoll(int roll) {
  if (roll <= rollGreenMax) {
    return map(roll, 0, rollGreenMax, 0, rollGreenLeds);
  } else if (roll <= rollYellowMax) {
    return map(roll, rollGreenMax, rollYellowMax, rollGreenLeds, rollYellowLeds);
  } else {
    return map(roll, rollYellowMax, rollMax, rollYellowLeds, rollMax);
  }
}

// Print the 32 bytes starting from 5A A5 for debugging
void printReceivedData(int startIndex) {
  Serial.println("32 bytes starting from 5A A5:");
  for (int i = 0; i < 32; i++) {
    if (receivedData[startIndex + i] < 0x10) Serial.print("0");
    Serial.print(receivedData[startIndex + i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

// Shift data in the buffer to start from the new index
void shiftData(int startIdx, int numBytes) {
  for (int i = 0; i < numBytes; i++) {
    receivedData[i] = receivedData[startIdx + i];
  }
}

// Convert bytes to numbers for pitch and roll
void convertBytesToNumbers(int startIndex) {
  int16_t roll = (receivedData[startIndex + 8] << 8) | receivedData[startIndex + 9];
  int16_t pitch = (receivedData[startIndex + 10] << 8) | receivedData[startIndex + 11];

  Serial.print("Roll: ");
  Serial.println(roll);
  Serial.print("Pitch: ");
  Serial.println(pitch);

  clear_all();

  // Handle pitch
  int pitchLeds = (abs(pitch) <= pitchMax) ? mapPitch(abs(pitch)) : pitchMax;
  glowLed(pitch >= 0 ? posPitchStrip : negPitchStrip, pitchLeds, "pitch");

  // Handle roll
  int rollLeds = (abs(roll) <= rollMax) ? mapRoll(abs(roll)) : rollMax;
  glowLed(roll >= 0 ? posRollStrip : negRollStrip, rollLeds, "roll");

  delay(10);
}

void clear_all() {
  clear_leds(posPitchStrip, maxPitchLeds - 2);
  clear_leds(negPitchStrip, maxPitchLeds - 2);
  clear_leds(posRollStrip, maxRollLeds - 2);
  clear_leds(negRollStrip, maxRollLeds - 2);
}

// Helper function to initialize the LED strips
void initLedStrip(Adafruit_NeoPixel &strip, int maxLeds) {
  strip.begin();
  strip.setPixelColor(maxLeds - 1, white);
  strip.setBrightness(200);
  strip.show();
}
