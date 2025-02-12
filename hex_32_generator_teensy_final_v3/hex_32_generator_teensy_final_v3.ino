#define ENCODER1_PIN_A 2
#define ENCODER1_PIN_B 3
#define BUTTON1_PIN 4

#define ENCODER2_PIN_A 5
#define ENCODER2_PIN_B 6
#define BUTTON2_PIN 7

volatile float encoder1Position = 0.0;  // Use float for decimal precision
volatile float encoder2Position = 0.0;
volatile bool encoder1Moved = false;
volatile bool encoder2Moved = false;

const float maxPosition = 90.0;
const float minPosition = -90.0;

float roll = 0.0;
float pitch = 0.0;
int rollResolution = 0;
int pitchResolution = 0;

byte highByteValueRoll = 0x00;
byte lowByteValueRoll = 0x00;
byte highByteValuePitch = 0x00;
byte lowByteValuePitch = 0x00;

// Constants for mock data
#define HEADER_HIGH 0x5A
#define HEADER_LOW 0xA5
#define DATA_LENGTH 0x1A
#define IDENTIFIER 0x01
#define STATUS1 0x00
#define STATUS2 0x00
#define TERMINATOR 0xAA

void setup() {
  pinMode(ENCODER1_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER1_PIN_B, INPUT_PULLUP);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);

  pinMode(ENCODER2_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER2_PIN_B, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER1_PIN_A), encoder1ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER2_PIN_A), encoder2ISR, CHANGE);

  Serial1.begin(115200);
  Serial.begin(9600);
}

void loop() {
  if (encoder1Moved || encoder2Moved) {
    roll = constrain(encoder1Position, minPosition, maxPosition);
    pitch = constrain(encoder2Position, minPosition, maxPosition);

    rollResolution = roll / 0.00274658;
    pitchResolution = pitch / 0.00274658;

    highByteValueRoll = (rollResolution >> 8) & 0xFF;
    lowByteValueRoll = rollResolution & 0xFF;
    highByteValuePitch = (pitchResolution >> 8) & 0xFF;
    lowByteValuePitch = pitchResolution & 0xFF;

    encoder1Moved = false;
    encoder2Moved = false;
  }

  // Create data packet
  byte data[32] = {
    HEADER_HIGH, HEADER_LOW, DATA_LENGTH, IDENTIFIER,  
    STATUS1, STATUS2,
    
    highByte(0x0000), lowByte(0x0000),  // Heading placeholder

    highByteValueRoll, lowByteValueRoll,  // Roll
    highByteValuePitch, lowByteValuePitch,  // Pitch

    highByte(0x0000), lowByte(0x0000),  // Heading Rate
    highByte(0x0000), lowByte(0x0000),  // Roll Rate
    highByte(0x0000), lowByte(0x0000),  // Pitch Rate
    highByte(0x0000), lowByte(0x0000),  // North Velocity
    highByte(0x0000), lowByte(0x0000),  // East Velocity
    highByte(0x0000), lowByte(0x0000),  // Down Velocity
    highByte(0x0000), lowByte(0x0000),  // North Acceleration
    highByte(0x0000), lowByte(0x0000),  // East Acceleration
    highByte(0x0000), lowByte(0x0000),  // Down Acceleration

    0x00,  // Checksum placeholder
    TERMINATOR
  };

  // Calculate checksum
  uint8_t sum = 0;
  for (int i = 2; i < 30; i++) {  
    sum += data[i];
  }
  data[30] = sum % 256;  // Store checksum

  // Print Data
  Serial.print("Data Packet: ");
  for (int i = 0; i < 32; i++) {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("Decimal Roll: ");
  Serial.println(rollResolution);
  Serial.print("Decimal Pitch: ");
  Serial.println(pitchResolution);

  Serial.print("Roll: ");
  Serial.println(roll);
  Serial.print("Pitch: ");
  Serial.println(pitch);

  // Send data to Serial1
  Serial1.write(data, 32);

  delay(10);
}

// Interrupt Service Routine for encoder 1
void encoder1ISR() {
  static int lastStateA1 = LOW;
  int currentStateA1 = digitalRead(ENCODER1_PIN_A);
  if (currentStateA1 != lastStateA1) {
    encoder1Position += (digitalRead(ENCODER1_PIN_B) != currentStateA1) ? 0.1 : -0.1;
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
    encoder2Position += (digitalRead(ENCODER2_PIN_B) != currentStateA2) ? 0.1 : -0.1;
    encoder2Position = constrain(encoder2Position, minPosition, maxPosition);
    encoder2Moved = true;
  }
  lastStateA2 = currentStateA2;
}
