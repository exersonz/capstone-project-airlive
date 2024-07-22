#ifndef IO_HPP
#define IO_HPP

#include <keys.hpp>

#include <AdafruitIO_WiFi.h>

namespace IO {

bool IO_connected{false};

// IO handler
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Feeds
AdafruitIO_Feed *voc = io.feed("voc");
AdafruitIO_Feed *category = io.feed("category");
AdafruitIO_Feed *humidity = io.feed("humidity");
AdafruitIO_Feed *temperature = io.feed("temperature");

// Attempt connection
void try_connect() {
  Serial.print("Connecting to Adafruit IO");

  io.connect();

  // waits for a connection for 10 seconds
  for (std::size_t i{0}; (i < 20) || (io.status() < AIO_CONNECTED); i++) {
    Serial.print(".");
    delay(500);
  }

  if (io.status() < AIO_CONNECTED) {
    Serial.println("Connection failed");
  } else {
    Serial.println("Connected to Adafruit IO\n");
    Serial.println(io.statusText());
    IO_connected = true;
  }
}

// run if connected
void run() { io.run(); }

}  // namespace IO

#endif  // IO_HPP