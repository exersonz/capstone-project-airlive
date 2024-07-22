#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <Arduino.h>
#include <Adafruit_BME680.h>
#include <Adafruit_SGP40.h>

namespace SENSOR {
    // Create SGP40 and BME688 objects
    Adafruit_SGP40 sgp;
    Adafruit_BME680 bme;

    /**
     * @brief Initializes the SGP40 and BME688 sensors
     * 
     * @return true if initialization is successful
     * @return false if initialization fails
     */
    bool initialize() {
        Serial.println("SGP40 test with BME688 compensation");

        // Initialize SGP40 sensor
        if (! sgp.begin()){
            Serial.println("SGP40 sensor not found :(");
            return false;
        }

        // Initialize BME688 sensor
        if (! bme.begin()) {   // Set to 0x45 for alternate i2c addr
            Serial.println("Couldn't find BME688");
            return false;
        }

        return true;
    }

    /**
     * @brief Get the temperature reading from the BME688 sensor
     * 
     * @return float 
     */
    float get_temperature() {
        return bme.readTemperature();
    }

    /**
     * @brief Get the humidity reading from the BME688 sensor
     * 
     * @return float 
     */
    float get_humidity() {
        return bme.readHumidity();
    }

    /**
     * @brief Gets the VOC index from the SGP40 sensor
     * 
     * @param temperature 
     * @param humidity 
     * @return int 
     */
    int get_voc_index(float temperature, float humidity) {
        return sgp.measureVocIndex(temperature, humidity);
    }

    /**
     * @brief Determines air quality category based on VOC index
     * 
     * @param voc_index 
     * @return String representing air quality category
     */
    String get_voc_category(int voc_index) {
        if (voc_index <= 100) return "Good";
        if (voc_index <= 200) return "Moderate";
        if (voc_index <= 300) return "Poor";
        return "Very Poor";
    }
}

#endif // SENSOR_HPP