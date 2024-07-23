#include <Arduino.h>

#include <coord.hpp>
#include <gps.hpp>
#include <io.hpp>
#include <sensors.hpp>
#include <screen.hpp>

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); } // Wait for serial console to open!

  // Initialize sensors
  if (!SENSOR::initialize()) {
    Serial.println("Failed to initialize sensors");
    while (1);
  }

  // Initialize peripherals
  GPS::initialize();
  screen::initialize();

  // Connect to AdafruitIO
  IO::try_connect();
}

void loop() {
  IO::run();

  auto my_coords{GPS::query_GPS()};

  float temp_c = SENSOR::get_temperature();
  float temp_f = (temp_c * 1.8) + 32;
  float humidity = SENSOR::get_humidity();
  float voc_index = SENSOR::get_voc_index(temp_c, humidity);
  String voc_category = SENSOR::get_voc_category(temp_c);

  if (IO::IO_connected) {
    IO::temperature->save(temp_f);
    IO::humidity->save(humidity);
    IO::voc->save(voc_index);    
    IO::category->save(voc_category, my_coords.latitude, my_coords.longitude, my_coords.altitude);
  }

  screen::display(temp_f, humidity, voc_index, voc_category);

  delay(300000);
}