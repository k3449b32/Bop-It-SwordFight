#define AnalogSend = A0;
#define AnalogRead = A1;
void setup() {
  // put your setup code here, to run once:
  CapacitiveSensor TouchSensor = CapacitiveSensor(AnalogSend, AnalogRead);
}

void loop() {
  // put your main code here, to run repeatedly:

}
