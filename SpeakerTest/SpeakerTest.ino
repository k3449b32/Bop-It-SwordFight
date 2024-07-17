#include <SD.h>
#include <TMRpcm.h>
#include <SPI.h>

#define SD_ChipSelectPin 6
TMRpcm tmrpcm;

void setup() {
  tmrpcm.speakerPin = 9; // Set the speaker pin (mono)
  Serial.begin(9600);    // Initialize serial communication for debugging

  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  tmrpcm.setVolume(5); // Adjust volume (0 to 7)
  tmrpcm.quality(2);   // Set 1 for 2x oversampling to improve quality

  // if (!tmrpcm.isPlaying()) {
  //   tmrpcm.play("test.wav"); // Play the WAV file
  //   Serial.println("Playing test.wav");
  // }
}

void loop() {
  if (!tmrpcm.isPlaying()) {
    tmrpcm.play("test.wav"); // Play the WAV file
    Serial.println("Playing test.wav");
  }
  delay(10);
}
