#include <Arduino.h>

#include <coord.hpp>
#include <gps.hpp>
#include <io.hpp>
#include <sensors.hpp>
#include <screen.hpp>

void handleMessage(AdafruitIO_Data *data);

bool is_connected = false; // track connection status

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
  screen::displayLoadingMessage("Connecting to AdafruitIO..");
  IO::try_connect();
  is_connected = IO::IO_connected;

  if (!is_connected) {
    screen::displayLoadingMessage("Going to \noffline mode");
  }

  // Fetch other feeds
  //IO::aileen->onMessage(handleMessage);
  //IO::hanna->onMessage(handleMessage);
}

void loop() {
  if (is_connected) {
    IO::run();
  }

  auto my_coords{GPS::query_GPS()};

  float temp_c = SENSOR::get_temperature();
  float temp_f = (temp_c * 1.8) + 32;
  float humidity = SENSOR::get_humidity();
  int32_t voc_index = SENSOR::get_voc_index(temp_c, humidity);
  String voc_category = SENSOR::get_voc_category(voc_index);

  String voc_data = voc_category + ": " + String(voc_index);
  String humidity_str = String(humidity) + " %RH";

  if (is_connected && IO::IO_connected) {
    IO::temperature->save(temp_f);
    IO::humidity->save(humidity_str);
    IO::voc->save(voc_index);    
    IO::collective_data->save(voc_data, 40.69, -73.98, 203.67);
  }

  screen::display(temp_f, temp_c, humidity_str, voc_index, voc_category);

  delay(10000);
}

void handleMessage(AdafruitIO_Data *data) {
  String received_data = data->toString();

  double received_lat = data->lat();
  double received_lon = data->lon();
  double received_ele = data->ele();

  IO::collective_data->save(received_data, received_lat, received_lon, received_ele);
}