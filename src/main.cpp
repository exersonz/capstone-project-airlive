#include <Arduino.h>

#include <coord.hpp>
#include <gps.hpp>
#include <io.hpp>
#include <sensors.hpp>
#include <screen.hpp>

void handleMessage(AdafruitIO_Data *data);

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

  // Fetch other feeds
  IO::aileen->onMessage(handleMessage);
  IO::hanna->onMessage(handleMessage);
}

void loop() {
  IO::run();

  auto my_coords{GPS::query_GPS()};

  float temp_c = SENSOR::get_temperature();
  float temp_f = (temp_c * 1.8) + 32;
  float humidity = SENSOR::get_humidity();
  float voc_index = SENSOR::get_voc_index(temp_c, humidity);
  String voc_category = SENSOR::get_voc_category(temp_c);

  String voc_data = voc_category + ": " + String(voc_index);
  String humidity_str = String(humidity) + "%RH";

  if (IO::IO_connected) {
    IO::temperature->save(temp_f);
    IO::humidity->save(humidity_str);
    IO::voc->save(voc_index);    
    IO::collective_data->save(voc_data, 35.68, 139.76, 131);
  }

  screen::display(temp_f, temp_c, humidity_str, voc_index, voc_category);

  delay(300000);
}

void handleMessage(AdafruitIO_Data *data) {
  String received_data = data->toString();

  double received_lat = data->lat();
  double received_lon = data->lon();
  double received_ele = data->ele();

  IO::collective_data->save(received_data, received_lat, received_lon, received_ele);
}