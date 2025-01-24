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
const int pitchGreenMax = 5;   // Green zone max angle
const int pitchYellowMax = 8;  // Yellow zone max angle
const int pitchMax = 28;       // Absolute max pitch angle
const int pitchGreenLeds = 8;  // LEDs in green zone
const int pitchYellowLeds = 15; // LEDs in yellow zone

// Roll angle thresholds and LED zone definitions
const int rollGreenMax = 8;    // Green zone max angle
const int rollYellowMax = 16;  // Yellow zone max angle
const int rollMax = 58;        // Absolute max roll angle
const int rollGreenLeds = 20;  // LEDs in green zone
const int rollYellowLeds = 40; // LEDs in yellow zone

// Variables to store calculated LED values for pitch and roll
int posPitchLeds = 0;
int negPitchLeds = 0;
int posRollLeds = 0;
int negRollLeds = 0;

// LED color constants
const int R = 255;
const int G = 255;
const int B = 255;

// Adafruit NeoPixel LED strips
Adafruit_NeoPixel posPitchStrip = Adafruit_NeoPixel(maxPitchLeds, posPitchPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel negPitchStrip = Adafruit_NeoPixel(maxPitchLeds, negPitchPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel posRollStrip = Adafruit_NeoPixel(maxRollLeds, posRollPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel negRollStrip = Adafruit_NeoPixel(maxRollLeds, negRollPin, NEO_GRB + NEO_KHZ800);

// Data buffer for serial communication
const int bufferSize = 64;
byte receivedData[bufferSize];
int bytesReceived = 0;

// Helper functions for LED strip operations
void yellow_fill(Adafruit_NeoPixel &led_strip, int endIndex);
void red_fill(Adafruit_NeoPixel &led_strip, int endIndex);
void clear_leds(Adafruit_NeoPixel &led_strip, int leds);
void glowLed(Adafruit_NeoPixel &led_strip, int leds, String direction);

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
  posPitchStrip.begin();
  negPitchStrip.begin();
  posRollStrip.begin();
  negRollStrip.begin();

  // Set boundary LEDs to glow continuously
  posPitchStrip.setPixelColor(maxPitchLeds - 1, posPitchStrip.Color(R, G, B));
  negPitchStrip.setPixelColor(maxPitchLeds - 1, negPitchStrip.Color(R, G, B));
  posRollStrip.setPixelColor(maxRollLeds - 1, posRollStrip.Color(R, G, B));
  negRollStrip.setPixelColor(maxRollLeds - 1, negRollStrip.Color(R, G, B));

  // Set brightness for LED strips
  posPitchStrip.setBrightness(100);
  negPitchStrip.setBrightness(100);
  posRollStrip.setBrightness(100);
  negRollStrip.setBrightness(100);

  // Show initial state
  posPitchStrip.show();
  negPitchStrip.show();
  posRollStrip.show();
  negRollStrip.show();

  Serial.begin(115200); // Serial Monitor at 9600 baud
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
    clear_leds(posPitchStrip, maxPitchLeds - 2);
    clear_leds(negPitchStrip, maxPitchLeds - 2);
    clear_leds(posRollStrip, maxRollLeds - 2);
    clear_leds(negRollStrip, maxRollLeds - 2);
  }
}

// Add function implementations below

// Fill LED strip with yellow color up to a specific index
void yellow_fill(Adafruit_NeoPixel &led_strip, int endIndex) {
  for (int i = 0; i <= endIndex; i++) {
    led_strip.setPixelColor(i, led_strip.Color(200, 150, 0)); // Yellow
  }
}

// Fill LED strip with red color up to a specific index
void red_fill(Adafruit_NeoPixel &led_strip, int endIndex) {
  for (int i = 0; i <= endIndex; i++) {
    led_strip.setPixelColor(i, led_strip.Color(255, 0, 0)); // Red
  }
}

// Clear all LEDs on a strip
void clear_leds(Adafruit_NeoPixel &led_strip, int leds) {
  for (int i = 0; i <= leds; i++) {
    led_strip.setPixelColor(i, led_strip.Color(0, 0, 0)); // Turn off LEDs
  }
  led_strip.show();
}

// Glow LEDs based on pitch or roll values
void glowLed(Adafruit_NeoPixel &led_strip, int leds, String direction) {
  for (int i = 0; i <= leds; i++) {
    if (direction == "pitch") {
      if (leds <= pitchGreenLeds) {
        led_strip.setPixelColor(i, led_strip.Color(0, 255, 0)); // Green
      } else if (leds > pitchGreenLeds && leds <= pitchYellowLeds) {
        yellow_fill(led_strip, pitchGreenLeds);
        led_strip.setPixelColor(i, led_strip.Color(200, 150, 0)); // Yellow
      } else {
        red_fill(led_strip, pitchYellowLeds);
        led_strip.setPixelColor(i, led_strip.Color(255, 0, 0)); // Red
      }
    } else if (direction == "roll") {
      if (leds <= rollGreenLeds) {
        led_strip.setPixelColor(i, led_strip.Color(0, 255, 0)); // Green
      } else if (leds > rollGreenLeds && leds <= rollYellowLeds) {
        yellow_fill(led_strip, rollGreenLeds);
        led_strip.setPixelColor(i, led_strip.Color(200, 150, 0)); // Yellow
      } else {
        red_fill(led_strip, rollYellowLeds);
        led_strip.setPixelColor(i, led_strip.Color(255, 0, 0)); // Red
      }
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

  clear_leds(posPitchStrip, maxPitchLeds - 2);
  clear_leds(negPitchStrip, maxPitchLeds - 2);
  clear_leds(posRollStrip, maxRollLeds - 2);
  clear_leds(negRollStrip, maxRollLeds - 2);

  if (pitch >= -pitchMax && pitch <= pitchMax) {
    if (pitch >= 0) {
      posPitchLeds = mapPitch(pitch);
      glowLed(posPitchStrip, posPitchLeds, "pitch");
    } else {
      negPitchLeds = mapPitch(abs(pitch));
      glowLed(negPitchStrip, negPitchLeds, "pitch");
    }
  }

  if (roll >= -rollMax && roll <= rollMax) {
    if (roll >= 0) {
      posRollLeds = mapRoll(roll);
      glowLed(posRollStrip, posRollLeds, "roll");
    } else {
      negRollLeds = mapRoll(abs(roll));
      glowLed(negRollStrip, negRollLeds, "roll");
    }
  }
  delay(10);
}
