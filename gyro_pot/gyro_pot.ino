// Potentiometer Example Code

#define POTENTIOMETER1_PIN 14  // Analog pin connected to the potentiometer
#define POTENTIOMETER2_PIN 15


void setup() {
  Serial.begin(9600);  // Initialize the Serial Monitor at 9600 baud rate
  Serial1.begin(9600);
}

void loop() {
  int pot1mappedValue = 0;
  int pot2mappedValue = 0;
  int pot1Value = analogRead(POTENTIOMETER1_PIN);  // Read the potentiometer value (0-1023)
  int pot2Value = analogRead(POTENTIOMETER2_PIN);  // Read the potentiometer value (0-1023)

  if (pot1Value < 511) {
    pot1mappedValue = map(pot1Value, 0, 510, -90, 0);
  } else if (pot1Value > 511) {
    pot1mappedValue = map(pot1Value, 512, 1023, 0, 90);
  }

  if (pot2Value < 511) {
    pot2mappedValue = map(pot2Value, 0, 510, -90, 0);
  } else if (pot2Value > 511) {
    pot2mappedValue = map(pot2Value, 512, 1023, 0, 90); 
  }

  Serial.print(pot1mappedValue);
  Serial.print(",");
  Serial.println(pot2mappedValue);

  Serial1.print(pot1mappedValue);
  Serial1.print(",");
  Serial1.println(pot2mappedValue);
  // delay(100);  // Small delay for stability
}
