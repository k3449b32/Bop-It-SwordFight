#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

#define LED 7
const int AccThresh = 45;


void setup(void) {
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }
  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: +-8G");

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Serial.println("");
  delay(100);
}

void loop() {
  // Get new sensor events with the readings
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Print out the acceleration values
  //Serial.print("Acceleration X: ");
  //Serial.print(a.acceleration.x);
  if ((abs(a.acceleration.x) > AccThresh) || (abs(a.acceleration.y) > AccThresh) || (abs(a.acceleration.z) > AccThresh)) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  // Serial.print(", Y: ");
  // Serial.print(a.acceleration.y);
  // Serial.print(", Z: ");
  // Serial.print(a.acceleration.z);
  // Serial.println(" m/s^2");
  //delay(70);
}
