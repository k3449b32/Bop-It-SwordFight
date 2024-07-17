#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   11 // MOSI pin
#define OLED_CLK    13 // SCK (Serial Clock) pin
#define OLED_DC     10 // Arbitrary Digital
#define OLED_CS     8 // Arbitrary Digital
#define OLED_RESET  0  // We're not even using.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define NUMFLAKES     10 // Number of snowflakes in the animation example
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

#define TEXT_PRINT_DELAY 100

unsigned long drawChar_StMs = 0;
uint8_t charIndex = 0;
String currentStr = "";
bool gameStarted = false;
bool displayGameStart = false;

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
}

void writeText(String str) {
  if (charIndex == 0) {
    display.clearDisplay();
    centerText(display, str);
  }
  unsigned long currentMillis = millis();
  if (currentMillis - drawChar_StMs >= TEXT_PRINT_DELAY) {
    drawChar_StMs = currentMillis;
    if (charIndex < str.length()) {
      display.write(str[charIndex]);
      display.display();
      charIndex++;
    }
  }
  if (charIndex >= str.length()) {
    delay(1000);
    charIndex = 0;
    displayGameStart = false; // Reset the displayGameStart flag after the text is fully displayed
  }
}

void centerText(Adafruit_SSD1306 &display, String str) { // Access display object by address to modify
  int16_t x = 0, y = 0;
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(str, x, y, &x1, &y1, &w, &h);
  display.setCursor(x + SCREEN_WIDTH / 2 - w / 2, y + SCREEN_HEIGHT / 2 - h / 2);
}

void startIdleLoop() {
  if (!gameStarted && !displayGameStart) {
    display.setTextSize(2);
    currentStr = "Swing to\n  Play...";
    writeText(currentStr);
  }
}

void loop() {
  if (Serial.available() > 0) {
    char received = Serial.read();
    if (received == 'X') {
      gameStarted = true;
      displayGameStart = true;
      charIndex = 0; // Reset charIndex to start displaying "Game Start" from the beginning
    }
  }

  if (displayGameStart) {
    writeText("Game Start");
  } else {
    startIdleLoop();
  }
}
