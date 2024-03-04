#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
#define ANALOG_PIN 0
#define TIME_UNTIL_WARMUP 900

Adafruit_SSD1306 display(OLED_RESET);

class BreathAnalyzer {
  private:
    int analogPin;
    unsigned long warmupTime;
    
  public:
    BreathAnalyzer(int pin, unsigned long warmup) : analogPin(pin), warmupTime(warmup) {}
    
    void setup() {
      display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
      display.clearDisplay();
    }

    void loop() {
      delay(100);
      
      int val = readAlcohol();
      printTitle();
      
      if (millis() / 1000 <= warmupTime) {
        printWarming();
        displayWarmupProgress();
      } else {
        printAlcoholLevel(val);
      }

      display.display();
    }

  private:
    void printTitle() {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(22, 0);
      display.println("Breath Analyzer");
    }

    void printWarming() {
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 20);
      display.println("Warming up");
    }

    void displayWarmupProgress() {
      unsigned long time = millis() / 1000;
      int progress = map(time, 0, warmupTime, 0, 100);
      display.drawRect(10, 50, 110, 10, WHITE); // Empty Bar
      display.fillRect(10, 50, progress, 10, WHITE);
    }

    void printAlcoholLevel(int value) {
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(10, 55);
      
      if (value < 200) {
        display.println("You are sober.");
      } else if (value >= 200 && value < 280) {
        display.println("You had a beer.");
      } else if (value >= 280 && value < 350) {
        display.println("Two or more beers.");
      } else if (value >= 350 && value < 450) {
        display.println("I smell Ouzo!");
      } else {
        display.println("You are drunk!");
      }
    }

    int readAlcohol() {
      int val1 = analogRead(analogPin);
      delay(10);
      int val2 = analogRead(analogPin);
      delay(10);
      int val3 = analogRead(analogPin);
      
      return (val1 + val2 + val3) / 3;
    }
};

BreathAnalyzer breathAnalyzer(ANALOG_PIN, TIME_UNTIL_WARMUP);

void setup() {
  breathAnalyzer.setup();
}

void loop() {
  breathAnalyzer.loop();
}
