
// #define ENCODER1_PIN_A 2
// #define ENCODER1_PIN_B 3
// #define BUTTON1_PIN 4

// #define ENCODER2_PIN_A 5
// #define ENCODER2_PIN_B 6
// #define BUTTON2_PIN 7

// volatile int encoder1Position = 0;  // Position for encoder 1
// volatile int encoder2Position = 0;  // Position for encoder 2

// int lastStateA1;  // Last state of encoder 1 Pin A
// int lastStateA2;  // Last state of encoder 2 Pin A

// unsigned long lastDebounceTime1 = 0;     // Debounce timer for encoder 1
// unsigned long lastDebounceTime2 = 0;     // Debounce timer for encoder 2
// const unsigned long debounceDelay = 10;  // Debounce delay in milliseconds

// const int maxPosition = 90;   // Maximum positive range
// const int minPosition = -90;  // Maximum negative range

// bool encoder1Moved = false;  // Flag to track movement of encoder 1
// bool encoder2Moved = false;  // Flag to track movement of encoder 2

// int16_t roll = 0;   // Scaled roll value
// int16_t pitch = 0;  // Scaled pitch value

// // Constants for mock data
// #define HEADER_HIGH 0x5A
// #define HEADER_LOW 0xA5
// #define DATA_LENGTH 0x1A
// #define IDENTIFIER 0x01
// #define STATUS1 0xAA
// #define STATUS2 0xBB
// #define HEADING 0x1234
// #define HEADING_RATE 0x5678
// #define PITCH_RATE 0x1234
// #define ROLL_RATE 0x5678
// #define NORTH_VELOCITY 0x0000
// #define EAST_VELOCITY 0x0000
// #define DOWN_VELOCITY 0x0000
// #define NORTH_ACCEL 0x0000
// #define EAST_ACCEL 0x0000
// #define DOWN_ACCEL 0x0000
// #define TERMINATOR 0xAA

// void setup() {
//   pinMode(ENCODER1_PIN_A, INPUT_PULLUP);
//   pinMode(ENCODER1_PIN_B, INPUT_PULLUP);
//   pinMode(BUTTON1_PIN, INPUT_PULLUP);
//   lastStateA1 = digitalRead(ENCODER1_PIN_A);

//   pinMode(ENCODER2_PIN_A, INPUT_PULLUP);
//   pinMode(ENCODER2_PIN_B, INPUT_PULLUP);
//   pinMode(BUTTON2_PIN, INPUT_PULLUP);
//   lastStateA2 = digitalRead(ENCODER2_PIN_A);

//   Serial.begin(9600);  // Initialize Serial communication
// }

// void loop() {
//   // Handle encoders
//   handleEncoder(ENCODER1_PIN_A, ENCODER1_PIN_B, lastStateA1, encoder1Position, lastDebounceTime1, encoder1Moved);
//   handleEncoder(ENCODER2_PIN_A, ENCODER2_PIN_B, lastStateA2, encoder2Position, lastDebounceTime2, encoder2Moved);


//   if (encoder1Moved || encoder2Moved) {
//     const float scaleFactor = 90.0 / 32767.0;  // (90 degrees) / (2^15 - 1)

//     // Mock roll and pitch values using encoder positions
//     roll = encoder1Position * scaleFactor;   // Scaled roll value
//     pitch = encoder2Position * scaleFactor;  // Scaled pitch value

//     // Constrain roll and pitch to the range [-90, 90]
//     roll = constrain(roll, minPosition, maxPosition);
//     pitch = constrain(pitch, minPosition, maxPosition);
//     Serial.print("Encoder 1 Position: ");
//     Serial.print(encoder1Position);
//     Serial.print(" | Encoder 2 Position: ");
//     Serial.println(encoder2Position);
//     // Reset movement flags
//     encoder1Moved = false;
//     encoder2Moved = false;
//   }
//   // Create mock data based on the specified format
//   byte data[32] = {
//     HEADER_HIGH, HEADER_LOW, DATA_LENGTH, IDENTIFIER,   // Header and data length
//     STATUS1, STATUS2,                                   // Status bytes
//     highByte(HEADING), lowByte(HEADING),                // Heading
//     highByte(roll), lowByte(roll),                      // Attitude roll
//     highByte(pitch), lowByte(pitch),                    // Attitude pitch
//     highByte(HEADING_RATE), lowByte(HEADING_RATE),      // Heading rate
//     highByte(ROLL_RATE), lowByte(ROLL_RATE),            // Roll rate
//     highByte(PITCH_RATE), lowByte(PITCH_RATE),          // Pitch rate
//     highByte(NORTH_VELOCITY), lowByte(NORTH_VELOCITY),  // North velocity
//     highByte(EAST_VELOCITY), lowByte(EAST_VELOCITY),    // East velocity
//     highByte(DOWN_VELOCITY), lowByte(DOWN_VELOCITY),    // Down velocity
//     highByte(NORTH_ACCEL), lowByte(NORTH_ACCEL),        // North acceleration
//     highByte(EAST_ACCEL), lowByte(EAST_ACCEL),          // East acceleration
//     highByte(DOWN_ACCEL), lowByte(DOWN_ACCEL),          // Down acceleration
//     0x00,                                               // Checksum placeholder
//     TERMINATOR                                          // Terminator
//   };

//   // Calculate checksum
//   byte checksum = 0;
//   for (int i = 0; i < 31; i++) {
//     checksum += data[i];
//   }
//   data[31] = checksum;

//   // Send mock data over Serial
//   for (int i = 0; i < 32; i++) {
//     Serial.write(data[i]);
//   }

//   // delay(1000); // Send data at 1Hz
// }

// void handleEncoder(int pinA, int pinB, int &lastStateA, volatile int &encoderPosition, unsigned long &lastDebounceTime, bool &encoderMoved) {
//   int currentStateA = digitalRead(pinA);
//   if ((millis() - lastDebounceTime) > debounceDelay) {  // Debounce check
//     if (currentStateA != lastStateA) {
//       lastDebounceTime = millis();

//       // Determine direction
//       if (digitalRead(pinB) != currentStateA) {
//         encoderPosition++;  // Clockwise
//       } else {
//         encoderPosition--;  // Counterclockwise
//       }

//       // Constrain the position to the range [-90, 90]
//       encoderPosition = constrain(encoderPosition, minPosition, maxPosition);

//       // Set the movement flag
//       encoderMoved = true;
//     }
//   }
//   lastStateA = currentStateA;
// }
// #define ENCODER1_PIN_A 2
// #define ENCODER1_PIN_B 3
// #define BUTTON1_PIN 4

// #define ENCODER2_PIN_A 5
// #define ENCODER2_PIN_B 6
// #define BUTTON2_PIN 7

// volatile int encoder1Position = 0;  // Position for encoder 1
// volatile int encoder2Position = 0;  // Position for encoder 2

// int lastStateA1;  // Last state of encoder 1 Pin A
// int lastStateA2;  // Last state of encoder 2 Pin A

// unsigned long lastDebounceTime1 = 0;     // Debounce timer for encoder 1
// unsigned long lastDebounceTime2 = 0;     // Debounce timer for encoder 2
// const unsigned long debounceDelay = 10;  // Debounce delay in milliseconds

// const int maxPosition = 90;   // Maximum positive range
// const int minPosition = -90;  // Maximum negative range

// bool encoder1Moved = false;  // Flag to track movement of encoder 1
// bool encoder2Moved = false;  // Flag to track movement of encoder 2

// int16_t roll = 0;   // Scaled roll value
// int16_t pitch = 0;  // Scaled pitch value

// // Constants for mock data
// #define HEADER_HIGH 0x5A
// #define HEADER_LOW 0xA5
// #define DATA_LENGTH 0x1A
// #define IDENTIFIER 0x01
// #define STATUS1 0xAA
// #define STATUS2 0xBB
// #define HEADING 0x1234
// #define HEADING_RATE 0x5678
// #define PITCH_RATE 0x1234
// #define ROLL_RATE 0x5678
// #define NORTH_VELOCITY 0x0000
// #define EAST_VELOCITY 0x0000
// #define DOWN_VELOCITY 0x0000
// #define NORTH_ACCEL 0x0000
// #define EAST_ACCEL 0x0000
// #define DOWN_ACCEL 0x0000
// #define TERMINATOR 0xAA

// void setup() {
//   pinMode(ENCODER1_PIN_A, INPUT_PULLUP);
//   pinMode(ENCODER1_PIN_B, INPUT_PULLUP);
//   pinMode(BUTTON1_PIN, INPUT_PULLUP);
//   lastStateA1 = digitalRead(ENCODER1_PIN_A);

//   pinMode(ENCODER2_PIN_A, INPUT_PULLUP);
//   pinMode(ENCODER2_PIN_B, INPUT_PULLUP);
//   pinMode(BUTTON2_PIN, INPUT_PULLUP);
//   lastStateA2 = digitalRead(ENCODER2_PIN_A);

//   Serial.begin(9600);  // Initialize Serial communication
// }

// void loop() {
//   // Handle encoders
//   handleEncoder(ENCODER1_PIN_A, ENCODER1_PIN_B, lastStateA1, encoder1Position, lastDebounceTime1, encoder1Moved);
//   handleEncoder(ENCODER2_PIN_A, ENCODER2_PIN_B, lastStateA2, encoder2Position, lastDebounceTime2, encoder2Moved);

//   if (encoder1Moved || encoder2Moved) {
//     const float scaleFactor = 90.0 / 32767.0;  // (90 degrees) / (2^15 - 1)

//     // Mock roll and pitch values using encoder positions
//     roll = static_cast<int16_t>(encoder1Position * scaleFactor);   // Scaled roll value
//     pitch = static_cast<int16_t>(encoder2Position * scaleFactor);  // Scaled pitch value

//     // Constrain roll and pitch to the range [-90, 90]
//     roll = constrain(roll, minPosition, maxPosition);
//     pitch = constrain(pitch, minPosition, maxPosition);

//     Serial.print("Encoder 1 Position: ");
//     Serial.print(encoder1Position);
//     Serial.print(" | Encoder 2 Position: ");
//     Serial.println(encoder2Position);

//     // Reset movement flags
//     encoder1Moved = false;
//     encoder2Moved = false;
//   }

//   // Create mock data based on the specified format
//   byte data[32] = {
//     HEADER_HIGH, HEADER_LOW, DATA_LENGTH, IDENTIFIER,   // Header and data length
//     STATUS1, STATUS2,                                   // Status bytes
//     highByte(HEADING), lowByte(HEADING),                // Heading
//     highByte(roll), lowByte(roll),                      // Attitude roll
//     highByte(pitch), lowByte(pitch),                    // Attitude pitch
//     highByte(HEADING_RATE), lowByte(HEADING_RATE),      // Heading rate
//     highByte(ROLL_RATE), lowByte(ROLL_RATE),            // Roll rate
//     highByte(PITCH_RATE), lowByte(PITCH_RATE),          // Pitch rate
//     highByte(NORTH_VELOCITY), lowByte(NORTH_VELOCITY),  // North velocity
//     highByte(EAST_VELOCITY), lowByte(EAST_VELOCITY),    // East velocity
//     highByte(DOWN_VELOCITY), lowByte(DOWN_VELOCITY),    // Down velocity
//     highByte(NORTH_ACCEL), lowByte(NORTH_ACCEL),        // North acceleration
//     highByte(EAST_ACCEL), lowByte(EAST_ACCEL),          // East acceleration
//     highByte(DOWN_ACCEL), lowByte(DOWN_ACCEL),          // Down acceleration
//     0x00,                                               // Checksum placeholder
//     TERMINATOR                                          // Terminator
//   };

//   // Calculate checksum
//   byte checksum = 0;
//   for (int i = 0; i < 31; i++) {
//     checksum += data[i];
//   }
//   data[31] = checksum;

//   // Send mock data over Serial
//   //   // Send mock data over Serial
//   for (int i = 0; i < 32; i++) {
//     Serial.print(data[i], HEX);
//     Serial.print(" ");
//   }
//   Serial.println(" ");
//   delay(10);  // Send data at 1Hz
// }

// void handleEncoder(int pinA, int pinB, int &lastStateA, volatile int &encoderPosition, unsigned long &lastDebounceTime, bool &encoderMoved) {
//   int currentStateA = digitalRead(pinA);
//   if ((millis() - lastDebounceTime) > debounceDelay) {  // Debounce check
//     if (currentStateA != lastStateA) {
//       lastDebounceTime = millis();

//       // Determine direction
//       if (digitalRead(pinB) != currentStateA) {
//         encoderPosition++;  // Clockwise
//       } else {
//         encoderPosition--;  // Counterclockwise
//       }

//       // Constrain the position to the range [-90, 90]
//       encoderPosition = constrain(encoderPosition, minPosition, maxPosition);

//       // Set the movement flag
//       encoderMoved = true;
//     }
//   }
//   lastStateA = currentStateA;
// }



// #define ENCODER1_PIN_A 2
// #define ENCODER1_PIN_B 3
// #define BUTTON1_PIN 4

// #define ENCODER2_PIN_A 5
// #define ENCODER2_PIN_B 6
// #define BUTTON2_PIN 7

// volatile int encoder1Position = 0;  // Position for encoder 1
// volatile int encoder2Position = 0;  // Position for encoder 2

// int lastStateA1;  // Last state of encoder 1 Pin A
// int lastStateA2;  // Last state of encoder 2 Pin A

// unsigned long lastDebounceTime1 = 0;     // Debounce timer for encoder 1
// unsigned long lastDebounceTime2 = 0;     // Debounce timer for encoder 2
// const unsigned long debounceDelay = 10;  // Debounce delay in milliseconds

// const int maxPosition = 90;   // Maximum positive range
// const int minPosition = -90;  // Maximum negative range

// bool encoder1Moved = false;  // Flag to track movement of encoder 1
// bool encoder2Moved = false;  // Flag to track movement of encoder 2

// int16_t roll = 0;   // Scaled roll value
// int16_t pitch = 0;  // Scaled pitch value

// // Constants for mock data
// #define HEADER_HIGH 0x5A
// #define HEADER_LOW 0xA5
// #define DATA_LENGTH 0x1A
// #define IDENTIFIER 0x01
// #define STATUS1 0xAA
// #define STATUS2 0xBB
// #define HEADING 0x1234
// #define HEADING_RATE 0x5678
// #define PITCH_RATE 0x1234
// #define ROLL_RATE 0x5678
// #define NORTH_VELOCITY 0x0000
// #define EAST_VELOCITY 0x0000
// #define DOWN_VELOCITY 0x0000
// #define NORTH_ACCEL 0x0000
// #define EAST_ACCEL 0x0000
// #define DOWN_ACCEL 0x0000
// #define TERMINATOR 0xAA

// void setup() {
//   pinMode(ENCODER1_PIN_A, INPUT_PULLUP);
//   pinMode(ENCODER1_PIN_B, INPUT_PULLUP);
//   pinMode(BUTTON1_PIN, INPUT_PULLUP);
//   lastStateA1 = digitalRead(ENCODER1_PIN_A);

//   pinMode(ENCODER2_PIN_A, INPUT_PULLUP);
//   pinMode(ENCODER2_PIN_B, INPUT_PULLUP);
//   pinMode(BUTTON2_PIN, INPUT_PULLUP);
//   lastStateA2 = digitalRead(ENCODER2_PIN_A);

//   // Serial.begin(9600);  // Initialize Serial communication
//   Serial1.begin(9600);
// }

// void loop() {
//   // Handle encoders
//   handleEncoder(ENCODER1_PIN_A, ENCODER1_PIN_B, lastStateA1, encoder1Position, lastDebounceTime1, encoder1Moved);
//   handleEncoder(ENCODER2_PIN_A, ENCODER2_PIN_B, lastStateA2, encoder2Position, lastDebounceTime2, encoder2Moved);

//   if (encoder1Moved || encoder2Moved) {
//     roll = constrain(encoder1Position, minPosition, maxPosition);
//     pitch = constrain(encoder2Position, minPosition, maxPosition);

//     // Reset movement flags
//     encoder1Moved = false;
//     encoder2Moved = false;
//   }

//   // Create mock data based on the specified format
//   byte data[32] = {
//     HEADER_HIGH, HEADER_LOW, DATA_LENGTH, IDENTIFIER,   // Header and data length
//     STATUS1, STATUS2,                                   // Status bytes
//     highByte(HEADING), lowByte(HEADING),                // Heading
//     highByte(roll), lowByte(roll),                      // Attitude roll
//     highByte(pitch), lowByte(pitch),                    // Attitude pitch
//     highByte(HEADING_RATE), lowByte(HEADING_RATE),      // Heading rate
//     highByte(ROLL_RATE), lowByte(ROLL_RATE),            // Roll rate
//     highByte(PITCH_RATE), lowByte(PITCH_RATE),          // Pitch rate
//     highByte(NORTH_VELOCITY), lowByte(NORTH_VELOCITY),  // North velocity
//     highByte(EAST_VELOCITY), lowByte(EAST_VELOCITY),    // East velocity
//     highByte(DOWN_VELOCITY), lowByte(DOWN_VELOCITY),    // Down velocity
//     highByte(NORTH_ACCEL), lowByte(NORTH_ACCEL),        // North acceleration
//     highByte(EAST_ACCEL), lowByte(EAST_ACCEL),          // East acceleration
//     highByte(DOWN_ACCEL), lowByte(DOWN_ACCEL),          // Down acceleration
//     0x00,                                               // Checksum placeholder
//     TERMINATOR                                          // Terminator
//   };
//   data[8] = highByte(roll);
//   data[9] = lowByte(roll);
//   data[10] = highByte(pitch);
//   data[11] = lowByte(pitch);



//   // Calculate checksum
//   byte checksum = 0;
//   for (int i = 0; i < 30; i++) {
//     checksum += data[i];
//   }
//   data[30] = checksum;
//   Serial1.write(data, 32);

//   // Serial.print("Encoder 1 Position: ");
//   // Serial.println(encoder1Position);
//   // Serial.print("Encoder 2 Position: ");
//   // Serial.println(encoder2Position);
//   // Serial.println();
//   delay(10);  // Send data at 1 Hz
// }

// void handleEncoder(int pinA, int pinB, int &lastStateA, volatile int &encoderPosition, unsigned long &lastDebounceTime, bool &encoderMoved) {
//   int currentStateA = digitalRead(pinA);
//   if ((millis() - lastDebounceTime) > debounceDelay) {  // Debounce check
//     if (currentStateA != lastStateA) {
//       lastDebounceTime = millis();

//       // Determine direction
//       if (digitalRead(pinB) != currentStateA) {
//         encoderPosition++;  // Clockwise
//       } else {
//         encoderPosition--;  // Counterclockwise
//       }

//       // Constrain the position to the range [-90, 90]
//       encoderPosition = constrain(encoderPosition, minPosition, maxPosition);

//       // Set the movement flag
//       encoderMoved = true;
//     }
//   }
//   lastStateA = currentStateA;
// }


#define ENCODER1_PIN_A 2
#define ENCODER1_PIN_B 3
#define BUTTON1_PIN 4

#define ENCODER2_PIN_A 5
#define ENCODER2_PIN_B 6
#define BUTTON2_PIN 7

volatile int encoder1Position = 0;  // Position for encoder 1
volatile int encoder2Position = 0;  // Position for encoder 2

volatile bool encoder1Moved = false;  // Flag to track movement of encoder 1
volatile bool encoder2Moved = false;  // Flag to track movement of encoder 2

const int maxPosition = 90;   // Maximum positive range
const int minPosition = -90;  // Maximum negative range

int16_t roll = 0;   // Scaled roll value
int16_t pitch = 0;  // Scaled pitch value

// Constants for mock data
#define HEADER_HIGH 0x5A
#define HEADER_LOW 0xA5
#define DATA_LENGTH 0x1A
#define IDENTIFIER 0x01
#define STATUS1 0xAA
#define STATUS2 0xBB
#define HEADING 0x1234
#define HEADING_RATE 0x5678
#define PITCH_RATE 0x1234
#define ROLL_RATE 0x5678
#define NORTH_VELOCITY 0x0000
#define EAST_VELOCITY 0x0000
#define DOWN_VELOCITY 0x0000
#define NORTH_ACCEL 0x0000
#define EAST_ACCEL 0x0000
#define DOWN_ACCEL 0x0000
#define TERMINATOR 0xAA

void setup() {
  pinMode(ENCODER1_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER1_PIN_B, INPUT_PULLUP);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);

  pinMode(ENCODER2_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER2_PIN_B, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  // Attach interrupts for encoder 1 and encoder 2
  attachInterrupt(digitalPinToInterrupt(ENCODER1_PIN_A), encoder1ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER2_PIN_A), encoder2ISR, CHANGE);

  Serial1.begin(115200);  // Initialize Serial1 for communication
}

void loop() {
  // Check if encoder movement flags are set
  if (encoder1Moved || encoder2Moved) {
    roll = constrain(encoder1Position, minPosition, maxPosition);
    pitch = constrain(encoder2Position, minPosition, maxPosition);

    // Reset movement flags
    encoder1Moved = false;
    encoder2Moved = false;
  }

  // Create mock data based on the specified format
  byte data[32] = {
    HEADER_HIGH, HEADER_LOW, DATA_LENGTH, IDENTIFIER,   // Header and data length
    STATUS1, STATUS2,                                   // Status bytes
    highByte(HEADING), lowByte(HEADING),                // Heading
    highByte(roll), lowByte(roll),                      // Attitude roll
    highByte(pitch), lowByte(pitch),                    // Attitude pitch
    highByte(HEADING_RATE), lowByte(HEADING_RATE),      // Heading rate
    highByte(ROLL_RATE), lowByte(ROLL_RATE),            // Roll rate
    highByte(PITCH_RATE), lowByte(PITCH_RATE),          // Pitch rate
    highByte(NORTH_VELOCITY), lowByte(NORTH_VELOCITY),  // North velocity
    highByte(EAST_VELOCITY), lowByte(EAST_VELOCITY),    // East velocity
    highByte(DOWN_VELOCITY), lowByte(DOWN_VELOCITY),    // Down velocity
    highByte(NORTH_ACCEL), lowByte(NORTH_ACCEL),        // North acceleration
    highByte(EAST_ACCEL), lowByte(EAST_ACCEL),          // East acceleration
    highByte(DOWN_ACCEL), lowByte(DOWN_ACCEL),          // Down acceleration
    0x00,                                               // Checksum placeholder
    TERMINATOR                                          // Terminator
  };
  data[8] = highByte(roll);
  data[9] = lowByte(roll);
  data[10] = highByte(pitch);
  data[11] = lowByte(pitch);

  // Calculate checksum
  byte checksum = 0;
  for (int i = 0; i < 30; i++) {
    checksum += data[i];
  }
  data[30] = checksum;

  // Send data to Serial1
  Serial1.write(data, 32);

  delay(10);  // Adjust delay as needed for speed
}

// Interrupt Service Routine for encoder 1
void encoder1ISR() {
  static int lastStateA1 = LOW;
  int currentStateA1 = digitalRead(ENCODER1_PIN_A);
  if (currentStateA1 != lastStateA1) {
    // Check encoder direction by reading the state of encoder B
    if (digitalRead(ENCODER1_PIN_B) != currentStateA1) {
      encoder1Position++;  // Clockwise rotation
    } else {
      encoder1Position--;  // Counterclockwise rotation
    }

    // Constrain the encoder position to the range [-90, 90]
    encoder1Position = constrain(encoder1Position, minPosition, maxPosition);

    encoder1Moved = true;
  }
  lastStateA1 = currentStateA1;
}

// Interrupt Service Routine for encoder 2
void encoder2ISR() {
  static int lastStateA2 = LOW;
  int currentStateA2 = digitalRead(ENCODER2_PIN_A);
  if (currentStateA2 != lastStateA2) {
    // Check encoder direction by reading the state of encoder B
    if (digitalRead(ENCODER2_PIN_B) != currentStateA2) {
      encoder2Position++;  // Clockwise rotation
    } else {
      encoder2Position--;  // Counterclockwise rotation
    }

    // Constrain the encoder position to the range [-90, 90]
    encoder2Position = constrain(encoder2Position, minPosition, maxPosition);

    encoder2Moved = true;
  }
  lastStateA2 = currentStateA2;
}

