const int PACKET_SIZE = 32;
const byte HEADER_HIGH = 0x5A;
const byte HEADER_LOW = 0xA5;
const byte TERMINATOR = 0xAA;

byte receivedData[PACKET_SIZE];
int rollResolution, pitchResolution;
float roll, pitch;

void setup() {
  Serial1.begin(115200);  // Serial1 receives data from the sender
  Serial.begin(9600);     // Monitor output
}

void loop() {
  if (Serial1.available() >= PACKET_SIZE) {
    // Read the first two bytes to check the header
    if (Serial1.read() == HEADER_HIGH && Serial1.read() == HEADER_LOW) {
      receivedData[0] = HEADER_HIGH;
      receivedData[1] = HEADER_LOW;

      // Read the remaining bytes
      for (int i = 2; i < PACKET_SIZE; i++) {
        receivedData[i] = Serial1.read();
      }

      // Verify checksum
      uint8_t sum = 0;
      for (int i = 2; i < 30; i++) {
        sum += receivedData[i];
      }

      if (sum % 256 == receivedData[30] && receivedData[31] == TERMINATOR) {
        Serial.println("Valid packet received!");
        Serial.print("Data Packet: ");
        for (int i = 0; i < 32; i++) {
          Serial.print(receivedData[i], HEX);
          Serial.print(" ");
        }
        Serial.println();

        // Extract Roll and Pitch
        rollResolution = (int16_t)((receivedData[8] << 8) | receivedData[9]);
        pitchResolution = (int16_t)((receivedData[10] << 8) | receivedData[11]);


        roll = rollResolution * 0.00274658;
        pitch = pitchResolution * 0.00274658;

        Serial.print("Roll: ");
        Serial.println(roll);
        Serial.print("Pitch: ");
        Serial.println(pitch);
      } else {
        Serial.println("Checksum error!");
      }
    }
  }
}
