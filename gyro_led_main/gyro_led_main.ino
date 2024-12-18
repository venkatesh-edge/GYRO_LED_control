#include <Adafruit_NeoPixel.h>


Adafruit_NeoPixel positive_pitch_pixels = Adafruit_NeoPixel(15, 2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel negitive_pitch_pixels = Adafruit_NeoPixel(15, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel positive_roll_pixels = Adafruit_NeoPixel(15, 4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel negitive_roll_pixels = Adafruit_NeoPixel(15, 5, NEO_GRB + NEO_KHZ800);



int pitch = 0;  // Variable to store the number of LEDs to light up
int red = 255;
int green = 0;
int blue = 0;
bool pitch_sign_track = true;
bool roll_sign_track = true;



// pitch clear
void clear_pitch_Leds() {
  for (int i = 0; i < 15; i++) {
    positive_pitch_pixels.setPixelColor(i, positive_pitch_pixels.Color(0, 0, 0));  // Turn off all LEDs
  }
  positive_pitch_pixels.show();
  for (int i = 0; i < 15; i++) {
    negitive_pitch_pixels.setPixelColor(i, negitive_pitch_pixels.Color(0, 0, 0));  // Turn off all LEDs
  }
  negitive_pitch_pixels.show();
}

// roll clear
void clear_roll_Leds() {
  for (int i = 0; i < 15; i++) {
    positive_roll_pixels.setPixelColor(i, positive_roll_pixels.Color(0, 0, 0));  // Turn off all LEDs
  }
  positive_roll_pixels.show();
  for (int i = 0; i < 15; i++) {
    negitive_roll_pixels.setPixelColor(i, negitive_roll_pixels.Color(0, 0, 0));  // Turn off all LEDs
  }
  negitive_roll_pixels.show();
}


// pitch led control
void glowPitchLed(int pitch, bool pitch_sign_track) {
  if (pitch_sign_track == true) {
    clear_pitch_Leds();
    for (int i = 0; i < pitch; i++) {
      // positive_pitch_pixels.setPixelColor(i, positive_pitch_pixels.Color(255, 0, 0));  // Red color
      if (i < 5) {
        positive_pitch_pixels.setPixelColor(i, positive_pitch_pixels.Color(0, 255, 0));
      } else if (i > 4 && i < 10) {
        positive_pitch_pixels.setPixelColor(i, positive_pitch_pixels.Color(200, 150, 0));
      } else {
        positive_pitch_pixels.setPixelColor(i, positive_pitch_pixels.Color(255, 0, 0));
      }
    }
    positive_pitch_pixels.show();  // Update the LEDs
  } else {
    clear_pitch_Leds();
    for (int i = 0; i < pitch; i++) {
      // negitive_pitch_pixels.setPixelColor(i, negitive_pitch_pixels.Color(255, 0, 0));  // Red color
      if (i < 5) {
        negitive_pitch_pixels.setPixelColor(i, negitive_pitch_pixels.Color(0, 255, 0));
      } else if (i > 4 && i < 10) {
        negitive_pitch_pixels.setPixelColor(i, negitive_pitch_pixels.Color(200, 150, 0));
      } else {
        negitive_pitch_pixels.setPixelColor(i, negitive_pitch_pixels.Color(255, 0, 0));
      }
    }
    negitive_pitch_pixels.show();  // Update the LEDs
  }
}

// roll led control
void glowRollLed(int roll, bool roll_sign_track) {
  if (roll_sign_track == true) {
    clear_roll_Leds();
    for (int i = 0; i < roll; i++) {
      // positive_roll_pixels.setPixelColor(i, positive_roll_pixels.Color(255, 0, 0));  // Red color
      if (i < 5) {
        positive_roll_pixels.setPixelColor(i, positive_roll_pixels.Color(0, 255, 0));
      } else if (i > 4 && i < 10) {
        positive_roll_pixels.setPixelColor(i, positive_roll_pixels.Color(200, 150, 0));
      } else {
        positive_roll_pixels.setPixelColor(i, positive_roll_pixels.Color(255, 0, 0));
      }
    }
    positive_roll_pixels.show();  // Update the LEDs
  } else {
    clear_roll_Leds();
    for (int i = 0; i < roll; i++) {
      // negitive_roll_pixels.setPixelColor(i, negitive_roll_pixels.Color(255, 0, 0));  // Red color
      if (i < 5) {
        negitive_roll_pixels.setPixelColor(i, negitive_roll_pixels.Color(0, 255, 0));
      } else if (i > 4 && i < 10) {
        negitive_roll_pixels.setPixelColor(i, negitive_roll_pixels.Color(200, 150, 0));
      } else {
        negitive_roll_pixels.setPixelColor(i, negitive_roll_pixels.Color(255, 0, 0));
      }
    }
    negitive_roll_pixels.show();  // Update the LEDs
  }
}




void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  positive_pitch_pixels.begin();
  negitive_pitch_pixels.begin();
  positive_roll_pixels.begin();
  negitive_roll_pixels.begin();
  Serial.begin(9600);  // Start Serial communication
  Serial1.begin(9600);
  Serial.println("Enter the number of LEDs to light up (0-20, or negative value to light up the last LEDs):");
}




void loop() {
  String input = "";
  if (Serial.available() > 0) {
    // Read the number entered via Serial input
    input = Serial.readStringUntil('\n');  // Read input until a newline
  } else if (Serial1.available() > 0) {
    input = Serial1.readStringUntil('\n');
  }
  input.trim();                         // Remove any extra whitespace
  int commaIndex = input.indexOf(',');  // Find the comma separator

  if (commaIndex != -1) {                                 // Check if a comma exists
    String number1Str = input.substring(0, commaIndex);   // First number as string
    String number2Str = input.substring(commaIndex + 1);  // Second number as string

    int pitch = number1Str.toInt();  // Convert to integer
    int roll = number2Str.toInt();   // Convert to integer
    pitch = map(pitch, 0, 45, 0, 15);
    roll = map(roll, 0, 45, 0, 15);

    if (pitch >= 0) {
      pitch_sign_track = true;
      glowPitchLed(pitch, pitch_sign_track);
    } else if (pitch < 0) {
      pitch = abs(pitch);
      pitch_sign_track = false;
      glowPitchLed(pitch, pitch_sign_track);
    }

    if (roll >= 0) {
      roll_sign_track = true;
      glowRollLed(roll, roll_sign_track);
    } else if (roll < 0) {
      roll = abs(roll);
      roll_sign_track = false;
      glowRollLed(roll, roll_sign_track);
    }

    // Handle negative input: Light up the last LEDs based on the negative value

    // delay(100);  // Small delay to prevent excessive Serial reading
  }
}
