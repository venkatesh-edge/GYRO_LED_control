#include <Adafruit_NeoPixel.h>

int posPitchPin = 46, negPitchPin = 9, posRollPin = 10, negRollPin = 11, fault_led = 3;

// Maximum number of LEDs for pitch and roll LED strips
int maxPitchLeds = 30;
int maxRollLeds = 60;

// Pitch angle thresholds
int pitchGreenMax = 5;    // Max angle for green zone
int pitchYellowMax = 10;  // Max angle for yellow zone
int pitchMax = 28;              // Fixed maximum pitch angle, do not modify

// LED thresholds for pitch zones
int pitchGreenLeds = 10;   // LEDs in green zone
int pitchYellowLeds = 20;  // LEDs in yellow zone

// 0 to 5 -> mapped -> 0 to 10
// 5 to 10 -> mapped -> 10 to 20
// 10 to 28 -> mapped -> 20 to 28

// Roll angle thresholds
int rollGreenMax = 8;    // Max angle for green zone
int rollYellowMax = 16;  // Max angle for yellow zone
int rollMax = 58;              // Fixed maximum roll angle, do not modify

// LED thresholds for roll zones
int rollGreenLeds = 20;   // LEDs in green zone
int rollYellowLeds = 40;  // LEDs in yellow zone

// 0 to 8 -> mapped -> 0 to 20
// 8 to 16 -> mapped -> 20 to 40
// 16 to 58 -> mapped -> 40 to 58

// Variables to store calculated LED values for pitch and roll
int posPitchLeds = 0;
int negPitchLeds = 0;
int posRollLeds = 0;
int negRollLeds = 0;

// Color values for LEDs (Red, Green, Blue) for Boundary LED (White)
int R = 255;
int G = 255;
int B = 255;

// Initializing LED strips using Adafruit NeoPixel library
Adafruit_NeoPixel posPitchStrip = Adafruit_NeoPixel(maxPitchLeds, posPitchPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel negPitchStrip = Adafruit_NeoPixel(maxPitchLeds, negPitchPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel posRollStrip = Adafruit_NeoPixel(maxRollLeds, posRollPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel negRollStrip = Adafruit_NeoPixel(maxRollLeds, negRollPin, NEO_GRB + NEO_KHZ800);

// Function to fill a LED strip with yellow color up to a specified index
void yellow_fill(Adafruit_NeoPixel &led_strip, int endIndex) {
  for (int i = 0; i <= endIndex; i++) {
    led_strip.setPixelColor(i, led_strip.Color(200, 150, 0));  // Yellow color
  }
}

// Function to fill a LED strip with red color up to a specified index
void red_fill(Adafruit_NeoPixel &led_strip, int endIndex) {
  for (int i = 0; i <= endIndex; i++) {
    led_strip.setPixelColor(i, led_strip.Color(255, 0, 0));  // Red color
  }
}

// Function to clear all LEDs on a strip
void clear_leds(Adafruit_NeoPixel &led_strip, int leds) {
  for (int i = 0; i <= leds; i++) {
    led_strip.setPixelColor(i, led_strip.Color(0, 0, 0));  // Turn off all LEDs
  }
  led_strip.show();  // Apply the changes
}

// Function to glow LEDs based on pitch or roll values
void glowLed(Adafruit_NeoPixel &led_strip, int leds, String direction) {
  if (direction == "pitch") {  // Handling pitch LEDs
    for (int i = 0; i <= leds; i++) {
      if (leds <= pitchGreenLeds) {  // Green zone
        Serial.print("pitch green : ");
        Serial.println(i);
        led_strip.setPixelColor(i, led_strip.Color(0, 255, 0));           // Green color
      } else if (leds > pitchGreenLeds && leds <= pitchYellowLeds) {  // Yellow zone
        Serial.print("pitch yellow : ");
        Serial.println(i);
        yellow_fill(led_strip, pitchGreenLeds);                  // Fill green LEDs first
        led_strip.setPixelColor(i, led_strip.Color(200, 150, 0));  // Yellow color
      } else {                                                     // Red zone
        Serial.print("pitch red : ");
        Serial.println(i);
        red_fill(led_strip, pitchYellowLeds);                  // Fill green and yellow LEDs first
        led_strip.setPixelColor(i, led_strip.Color(255, 0, 0));  // Red color
      }
    }
  } else if (direction == "roll") {  // Handling roll LEDs
    for (int i = 0; i <= leds; i++) {
      if (leds <= rollGreenLeds) {  // Green zone
        Serial.print("roll green : ");
        Serial.println(i);
        led_strip.setPixelColor(i, led_strip.Color(0, 255, 0));         // Green color
      } else if (leds > rollGreenLeds && leds <= rollYellowLeds) {  // Yellow zone
        Serial.print("roll yellow : ");
        Serial.println(i);
        yellow_fill(led_strip, rollGreenLeds);                   // Fill green LEDs first
        led_strip.setPixelColor(i, led_strip.Color(200, 150, 0));  // Yellow color
      } else {                                                     // Red zone
        Serial.print("roll red : ");
        Serial.println(i);
        red_fill(led_strip, rollYellowLeds);                   // Fill green and yellow LEDs first
        led_strip.setPixelColor(i, led_strip.Color(255, 0, 0));  // Red color
      }
    }
  }
  led_strip.show();  // Apply the changes
}

// Map pitch angle to corresponding LEDs
int mapPitch(int pitch) {
  if (pitch <= pitchGreenMax) {  // Green zone
    return map(pitch, 0, pitchGreenMax, 0, pitchGreenLeds);
  } else if (pitch <= pitchYellowMax) {  // Yellow zone
    return map(pitch, pitchGreenMax, pitchYellowMax, pitchGreenLeds, pitchYellowLeds);
  } else {  // Red zone
    return map(pitch, pitchYellowMax, pitchMax, pitchYellowLeds, pitchMax);
  }
}

// Map roll angle to corresponding LEDs
int mapRoll(int roll) {
  if (roll <= rollGreenMax) {  // Green zone
    return map(roll, 0, rollGreenMax, 0, rollGreenLeds);
  } else if (roll <= rollYellowMax) {  // Yellow zone
    return map(roll, rollGreenMax, rollYellowMax, rollGreenLeds, rollYellowLeds);
  } else {  // Red zone
    return map(roll, rollYellowMax, rollMax, rollYellowLeds, rollMax);
  }
}

void setup() {
  pinMode(posPitchPin, OUTPUT);
  pinMode(negPitchPin, OUTPUT);
  pinMode(posRollPin, OUTPUT);
  pinMode(negRollPin, OUTPUT);
  pinMode(fault_led, OUTPUT);

  // Initialize the LED strips
  posPitchStrip.begin();
  negPitchStrip.begin();
  posRollStrip.begin();
  negRollStrip.begin();

  // Set the last LED in each strip to glow continuously
  posPitchStrip.setPixelColor(maxPitchLeds - 1, posPitchStrip.Color(R, G, B));  // should glow always, should not clear by clear function
  negPitchStrip.setPixelColor(maxPitchLeds - 1, negPitchStrip.Color(R, G, B));  // should glow always, should not clear by clear function
  posRollStrip.setPixelColor(maxRollLeds - 1, posRollStrip.Color(R, G, B));     // should glow always, should not clear by clear function
  negRollStrip.setPixelColor(maxRollLeds - 1, negRollStrip.Color(R, G, B));     // should glow always, should not clear by clear function

  // Show initial state
  posPitchStrip.show();
  negPitchStrip.show();
  posRollStrip.show();
  negRollStrip.show();
  Serial.begin(115200);  // Start Serial communication
  //Serial1.begin(115200);
  Serial.println("Enter the number of LEDs to light up (0-20, or negative value to light up the last LEDs):");
  //Serial1.println("Enter the number of LEDs to light up (0-20, or negative value to light up the last LEDs):");
}


void loop() {
  String input = "";             // Initialize an empty string to store input
  digitalWrite(fault_led, LOW);  // Ensure fault LED is off initially

  // Check if there is data available in the serial buffer
  if (Serial.available() > 0) {
    input = Serial.readStringUntil('\n');  // Read input until a newline
                                           // } else if (Serial1.available() > 0) {
    //input = Serial1.readStringUntil('\n');
  } else {
    // If no data is available, turn on the fault LED
    digitalWrite(fault_led, HIGH);
    // Clear LEDs if no data
    clear_leds(posPitchStrip, (maxPitchLeds - 2));
    clear_leds(negPitchStrip, (maxPitchLeds - 2));
    clear_leds(posRollStrip, (maxRollLeds - 2));
    clear_leds(negRollStrip, (maxRollLeds - 2));
  }
  input.trim();                         // Remove any extra whitespace from the input string
  int commaIndex = input.indexOf(',');  // Look for the comma separator in the input string

  // Check if the comma exists to separate pitch and roll values
  if (commaIndex != -1) {                                 // Check if a comma exists
    String number1Str = input.substring(0, commaIndex);   // First number as string
    String number2Str = input.substring(commaIndex + 1);  // Second number as stri1ng

    int pitch = number1Str.toInt();  // Convert to integer
    int roll = number2Str.toInt();   // Convert to integer

    // Print the received pitch and roll values for debugging
    Serial.print("Pitch, Roll: ");
    Serial.print(pitch);
    Serial.print(", ");
    Serial.println(roll);

    // Clear previous LED states before setting new ones
    clear_leds(posPitchStrip, (maxPitchLeds - 2));
    clear_leds(negPitchStrip, (maxPitchLeds - 2));
    clear_leds(posRollStrip, (maxRollLeds - 2));
    clear_leds(negRollStrip, (maxRollLeds - 2));

    // Process pitch values if they are within the valid range
    if (pitch >= -pitchMax && pitch <= pitchMax) {
      if (pitch >= 0) {  // Positive pitch value
        Serial.println("positive pitch");
        posPitchLeds = mapPitch(pitch);  // Map the pitch value to LED count
        Serial.print("Positive pitch mapped to LEDs: ");
        Serial.println(posPitchLeds);
        // Glow LEDs for positive pitch
        glowLed(posPitchStrip, posPitchLeds, "pitch");
      } else {
        Serial.println("negative pitch");
        // Map negative pitch to LEDs
        int abs_pitch = abs(pitch);                 // Convert negative pitch to positive for mapping
        negPitchLeds = mapPitch(abs_pitch);  // Map the absolute pitch to LED count
        Serial.print("Negative pitch mapped to LEDs: ");
        Serial.println(negPitchLeds);
        // Glow LEDs for negative pitch
        glowLed(negPitchStrip, negPitchLeds, "pitch");
      }
    } else {
      // Handle pitch values outside the defined range
      if (pitch > pitchMax) {
        Serial.println("positive pitch out of range, clamping to max");
        glowLed(posPitchStrip, pitchMax, "pitch");  // Clamp to max pitch
      } else if (pitch < -pitchMax) {
        Serial.println("negative pitch out of range, clamping to max");
        glowLed(negPitchStrip, pitchMax, "pitch");  // Clamp to min pitch
      }
    }

    // Debug output for the number of LEDs that should be lit for both positive and negative pitch
    Serial.print("posPitchPin_led: ");
    Serial.print(posPitchLeds);
    Serial.print(", negative_pitch_led: ");
    Serial.println(negPitchLeds);


    // Process roll values if they are within the valid range
    if (roll >= -rollMax && roll <= rollMax) {
      if (roll >= 0) {  // Positive roll value
        Serial.println("positive roll");
        posRollLeds = mapRoll(roll);  // Map the roll value to LED count
        Serial.print("Positive roll mapped to LEDs: ");
        Serial.println(posRollLeds);
        // Glow the LEDs for positive roll
        glowLed(posRollStrip, posRollLeds, "roll");
      } else {  // Negative roll value
        Serial.println("negative roll");
        int abs_roll = abs(roll);                // Convert negative roll to positive for mapping
        negRollLeds = mapRoll(abs_roll);  // Map the absolute roll to LED count
        Serial.print("Negative roll mapped to LEDs: ");
        Serial.println(negRollLeds);
        // Glow the LEDs for negative roll
        glowLed(negRollStrip, negRollLeds, "roll");
      }
    } else {
      // Handle out-of-range roll values
      if (roll > rollMax) {
        Serial.println("positive roll out of range, clamping to max");
        glowLed(posRollStrip, rollMax, "roll");  // Clamp to max roll
      } else if (roll < -rollMax) {
        Serial.println("negative roll out of range, clamping to max");
        glowLed(negRollStrip, rollMax, "roll");  // Clamp to min roll
      }
    }

    // Debug output for both positive and negative roll LEDs
    Serial.print("posRollPin, negative_roll_led: ");
    Serial.print(posRollLeds);
    Serial.print(", ");
    Serial.println(negRollLeds);

    // Print pitch and roll for debugging
    Serial.print("pitch, roll: ");
    Serial.print(pitch);
    Serial.print(", ");
    Serial.println(roll);
  }
}
