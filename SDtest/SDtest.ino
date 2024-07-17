#include <SD.h>
#include <SPI.h>

#define SD_ChipSelectPin 6

void setup() {
  Serial.begin(9600);
  pinMode(SD_ChipSelectPin, OUTPUT); // Ensure the CS pin is set as an output

  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  // List all files on the SD card (optional)
  File root = SD.open("/");
  printDirectory(root, 0);
}

void loop() {
  // Nothing needed here
}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // No more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // Files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
