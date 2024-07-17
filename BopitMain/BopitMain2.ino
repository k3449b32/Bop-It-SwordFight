#include <SPI.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiSpi.h>

Adafruit_MPU6050 mpu;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define CHAR_WIDTH 8     // Width of each character in pixels
#define CHAR_HEIGHT 16   // Height of each character in pixels
#define BAR_WIDTH 16
#define triggerPin 4
#define SPEAKER_PIN 9

// OLED display SPI pins
#define OLED_CS 8
#define OLED_DC 10
#define OLED_RESET -1 // Not used

SSD1306AsciiSpi oled;

#include <SD.h>
#include <TMRpcm.h>

#define SD_ChipSelectPin 6
TMRpcm tmrpcm;

#include <CapacitiveSensor.h>
CapacitiveSensor TouchSensor = CapacitiveSensor(2, 3);
const int TouchThreshold = 150;

#define TEXT_PRINT_DELAY 100 // Speed of individually printing every character
unsigned long drawChar_StMs = 0;

uint8_t charIndex = 0;
String currentStr = "";

bool gameStarted = false;
bool displayGameStart = false;
bool countDown = false;
bool gameOver = false;
bool victory = false;

uint8_t score = 0;
int gameSpd = 10000;
float acceleration = 0.97;

void setup() {
  Serial.begin(9600);

  tmrpcm.speakerPin = SPEAKER_PIN; // Set the speaker pin (mono)

  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  tmrpcm.setVolume(5); // Adjust volume (0 to 7)
  tmrpcm.quality(2);   // Set 1 for 2x oversampling to improve quality

  oled.begin(&Adafruit128x64, OLED_CS, OLED_DC, OLED_RESET);
  oled.setFont(ZevvPeep8x16);

  pinMode(triggerPin, INPUT);

  Serial.println(F("Searching for MPU6050"));
  if (!mpu.begin()) {
    while (1) {
      delay(10);
    }
  }
  Serial.println(F("MPU6050 Found!"));
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(4500);
  mpu.setMotionDetectionDuration(50);
  mpu.setInterruptPinLatch(false); // Keep it latched. Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  delay(100);
}

void writeTextInt(const String& str) {
  if (charIndex == 0) {
    noInterrupts();
    oled.clear();
    centerText(str);
    interrupts();
  }
  unsigned long currentMillis = millis();
  if (currentMillis - drawChar_StMs >= TEXT_PRINT_DELAY) {
    drawChar_StMs = currentMillis;
    if (charIndex < str.length()) {
      noInterrupts();
      oled.write(str[charIndex]);
      interrupts();
      charIndex++;
    }
  }
  if (charIndex >= str.length()) {
    delay(1000);
    charIndex = 0;
    displayGameStart = false; // Reset the displayGameStart flag after the text is fully displayed
  }
}

void centerText(const String& str) {
  int h = oled.displayRows();
  int16_t x = (SCREEN_WIDTH - (str.length() * CHAR_WIDTH)) / 2; // Calculate center column
  int16_t y = (SCREEN_HEIGHT - CHAR_HEIGHT) / CHAR_HEIGHT / 2 + 1; // Calculate center row
  oled.setCursor(x, y);
}

void startLoop() {
  while (!gameStarted) {
    if (digitalRead(triggerPin) == HIGH || mpu.getMotionInterruptStatus()) {
      gameStarted = true;
    }
    writeTextInt(F("Swing to Play"));
  }
}

void displayCountdownBar(unsigned long elapsedTime, unsigned long totalTime) {
  int barLength = BAR_WIDTH - (elapsedTime * BAR_WIDTH / totalTime);
  oled.setCursor(0, 7); // Set cursor to the bottom row
  for (int i = 0; i < BAR_WIDTH; i++) {
    if (i < barLength) {
      oled.write('|');
    } else {
      oled.write(' ');
    }
  }
}

void displayScore() {
  noInterrupts();
  oled.setCursor(110, 0); // Top right corner
  oled.print(score);
  interrupts();
}

void CNTDWN(uint8_t count) {
  if (gameStarted) {
    Serial.println(F("Starting Countdown"));
    for (int8_t i = count + 1; i >= 0; i--) {
      unsigned long startMS = millis();
      if (i != 0 && i <= count) {
        noInterrupts();
        oled.clear();
        displayScore();
        centerText(String(i));
        oled.print(String(i));
        interrupts();
      }
      if (i == count + 1) {
        String startString = F("  Starting Game.");
        noInterrupts();
        oled.clear();
        displayScore();
        centerText(startString);
        oled.print(startString);
        interrupts();
        tmrpcm.play("test.wav"); // Play the WAV file
        while (tmrpcm.isPlaying()) {}
      }
      if (i == 0) {
        String startString = F("PLAY!");
        Serial.println(startString);
        noInterrupts();
        oled.clear();
        displayScore();
        centerText(startString);
        oled.print(startString);
        interrupts();
        tmrpcm.play("go.wav"); // Play the WAV file
        while (tmrpcm.isPlaying()) {}
        noTone(SPEAKER_PIN);
      }
      while (millis() - startMS < 700) {
        if (i == 3) {
          tmrpcm.play("3.wav"); // Play the WAV file
          while (tmrpcm.isPlaying()) {}
          noTone(SPEAKER_PIN);
        } else if (i == 2) {
          tmrpcm.play("2.wav"); // Play the WAV file
          while (tmrpcm.isPlaying()) {}
          noTone(SPEAKER_PIN);
        } else if (i == 1) {
          tmrpcm.play("1.wav"); // Play the WAV file
          while (tmrpcm.isPlaying()) {}
          noTone(SPEAKER_PIN);
        }
      }
    }
  }
}

void loopCommand(){
  while(!gameOver and !victory){
    commandRand();
  }
  if(victory){
    String vic = F("VICTORY!");
    oled.clear();
    displayScore();
    centerText(vic);
    oled.print(vic);
  }
}

void commandRand() {
  uint8_t choice = random(0, 3);//3 is upper bound, ie: [0:3) for no reason :/
  switch (choice) {
    case 0:
      commandAcc();
      break;
    case 1:
      commandBunt();
      break;
    case 2:
      commandCharge();
      break;
  }
}

void commandAcc() {
  String command = F("Swing!");
  noInterrupts();
  oled.clear();
  displayScore();
  centerText(command);
  oled.print(command);
  delay(500);
  interrupts();
  unsigned long startMS = millis();
  while (1) {
    displayCountdownBar(millis() - startMS, gameSpd);
    if (mpu.getMotionInterruptStatus()) {
      score += 1;
      if(score == 100){
        victory = true;
      }
      displayScore(); // Update the counter display
      gameSpd = gameSpd*acceleration; // decrease time by 10% multiplicatively
      tmrpcm.play("coin.wav"); // Play the WAV file
      while (tmrpcm.isPlaying()) {}
      noTone(SPEAKER_PIN);
      break;
    }
    if (millis() - startMS > gameSpd) {
      gameOver = true;
      String gmOv = F("Game Over");
      oled.clear();
      displayScore();
      centerText(gmOv);
      oled.print(gmOv);
      break;
    }
  }
}

void commandBunt() {
  String command = F("Bunt!");
  noInterrupts();
  oled.clear();
  displayScore();
  centerText(command);
  oled.print(command);
  delay(500);//delay to prevent duplicate points
  interrupts();
  unsigned long startMS = millis();
  while (1) {
    displayCountdownBar(millis() - startMS, gameSpd);
    if (TouchSensor.capacitiveSensor(50) > TouchThreshold) {
      score += 1;
      displayScore(); // Update the counter display
      gameSpd = gameSpd*acceleration; // decrease time by 10% multiplicatively
      tmrpcm.play("coin.wav"); // Play the WAV file
      while (tmrpcm.isPlaying()) {}
      noTone(SPEAKER_PIN);
      break;
    }
    if (millis() - startMS > gameSpd) {
      gameOver = true;
      String gmOv = F("Game Over");
      oled.clear();
      displayScore();
      centerText(gmOv);
      oled.print(gmOv);
      break;
    }
  }
}

void commandCharge() {
  String command = F("Charge!");
  noInterrupts();
  oled.clear();
  displayScore();
  centerText(command);
  oled.print(command);
  delay(500);//delay to prevent duplicate points
  interrupts();
  unsigned long startMS = millis();
  int chargeCnt = 0;
  while (1) {
    displayCountdownBar(millis() - startMS, gameSpd);
    if (digitalRead(triggerPin) == HIGH) {
      chargeCnt++;
    }
    if (chargeCnt>=gameSpd/20) {
      score += 1;
      displayScore(); // Update the counter display
      gameSpd = gameSpd*acceleration; // decrease time by 10% multiplicatively
      tmrpcm.play("coin.wav"); // Play the WAV file
      while (tmrpcm.isPlaying()) {}
      noTone(SPEAKER_PIN);
      break;
    }
    if (millis() - startMS > gameSpd) {
      gameOver = true;
      String gmOv = F("Game Over");
      oled.clear();
      displayScore();
      centerText(gmOv);
      oled.print(gmOv);
      break;
    }
  }
}

void loop() {
  oled.setFont(ZevvPeep8x16);
  startLoop();
  CNTDWN(3);
  displayScore(); // Display the counter after the countdown
  loopCommand();

  // reset flags
  noInterrupts();
  score=0;
  noTone(SPEAKER_PIN);
  drawChar_StMs = 0;
  charIndex = 0;
  currentStr = "";
  gameStarted = false;
  displayGameStart = false;
  gameOver = false;
  victory = false;
  countDown = false;
  gameSpd = 10000;
  delay(3000);
  oled.clear();
  interrupts();
}
