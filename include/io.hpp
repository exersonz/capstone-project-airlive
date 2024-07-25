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

AdafruitIO_Feed *aileen = io.feed("aileen", "aileenchen781");
AdafruitIO_Feed *hanna = io.feed("hanna", "hanna_janusz");
AdafruitIO_Feed *collective_data = io.feed("collective-data");

// Attempt connection
void try_connect() {
  Serial.print("Connecting to Adafruit IO");

  io.connect();

  unsigned long start_time = millis();
  // waits for a connection for 10 seconds
  while ((millis() - start_time) < 10000) {
    if (io.status() >= AIO_CONNECTED) {
      Serial.println("Connected to AdafruitIO!");
      Serial.println(io.statusText());
      IO_connected = true;
      return;
    }
    Serial.print(".");
    delay(500);
  }

  Serial.println("Connection failed");
}

// run if connected
void run() { io.run(); }

}  // namespace IO

#endif  // IO_HPP