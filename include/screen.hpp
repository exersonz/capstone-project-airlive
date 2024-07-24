#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

namespace screen {

Adafruit_ST7789 screen = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void initialize();
void clear();

/**
 * @brief initialise screen
 */
void initialize() {
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  screen.init(135, 240);
  screen.setRotation(3);
  clear();
  screen.setTextSize(2);
}

/**
 * @brief clear screen
 */
void clear() {
  screen.fillScreen(ST77XX_BLACK);
  screen.setCursor(0, 10);
}

/**
 * @brief display info
 *
 * @param temp_f
 * @param temp_c
 * @param humidity
 * @param voc_index
 * @param voc_category
 */
void display(double temp_f, double temp_c, String humidity, int voc_index, String voc_category) {
  clear();
  screen.setTextColor(ST77XX_ORANGE);
  screen.println("AirLive");

  screen.setTextColor(ST77XX_CYAN);
  screen.println("--------------------");

  screen.setTextColor(ST77XX_WHITE);
  screen.print("Temp (F): "); screen.println(temp_f);
  screen.print("Temp (C): "); screen.println(temp_c);
  screen.print("Humidity: "); screen.println(humidity);
  screen.print("VOC Index: "); screen.println(voc_index);
  screen.print("VOC Category: "); screen.println(voc_category);
}

}  // namespace screen

#endif  // SCREEN_HPP