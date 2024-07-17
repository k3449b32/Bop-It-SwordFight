#include <CapacitiveSensor.h>
#define LED 7
CapacitiveSensor TouchSensor = CapacitiveSensor(2, 3);
const int TouchThreshold = 125;

void setup() {
  // put your setup code here, to run once:
  //TouchSensor.set_CS_AutocaL_Millis(0xFFFFFFFF);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  float start = millis();
  float total1 = TouchSensor.capacitiveSensor(15);
  char Value[] = "Touch Value: ";
  Serial.println("Reading Value");
  //Serial.println(total1);
  if (total1 > TouchThreshold){
    Serial.print(Value);      // check on performance in milliseconds
    Serial.println(total1);
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW); 
  }
}
