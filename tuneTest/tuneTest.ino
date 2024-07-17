#define SPEAKER_PIN 9
#define triggerPin 2
#define LEDpin 1
void setup() {
  // Start generating a 440 Hz tone on pin 9
  //tone(SPEAKER_PIN, 100); // Generates a tone of 440 Hz (A4 note)
  Serial.begin(9600);    // Initialize serial communication for debugging
  pinMode(triggerPin, INPUT);

}

void loop() {
  // Nothing needed here
  Serial.println(digitalRead(triggerPin));
  if (digitalRead(triggerPin)==1){
    digitalWrite(LEDpin, HIGH);
    delay(5000);
    digitalWrite(LEDpin, LOW);
    delay(10);
  }
  delay(20);
}
