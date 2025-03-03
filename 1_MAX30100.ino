#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define TCA9548A_ADDR 0x70  // I2C Address of TCA9548A
#define MAX30100_CHANNEL 7   // Channel where MAX30100 is connected

PulseOximeter pox;
uint32_t lastReportTime = 0;

// Function to select the specific channel on TCA9548A
void selectI2CChannel(uint8_t channel) {
    if (channel > 7) return; // TCA9548A supports channels 0-7
    Wire.beginTransmission(TCA9548A_ADDR);
    Wire.write(1 << channel);
    Wire.endTransmission();
}

// Callback function for beat detection
void onBeatDetected() {
    Serial.println("♥ Beat Detected!");
}

void setup() {
    Serial.begin(115200);
    Wire.begin(); // Initialize I2C

    // Select channel 7 where MAX30100 is connected
    selectI2CChannel(MAX30100_CHANNEL);
    delay(100);

    Serial.println("Initializing MAX30100...");

    // Initialize the MAX30100 sensor
    if (!pox.begin()) {
        Serial.println("MAX30100 initialization failed!");
        while (1); // Halt if the sensor is not found
    }

    pox.setOnBeatDetectedCallback(onBeatDetected);
    Serial.println("MAX30100 Initialized!");
}

void loop() {
    // Ensure channel selection remains active
    selectI2CChannel(MAX30100_CHANNEL);

    // Read data from MAX30100
    pox.update();

    // Print data every second
    if (millis() - lastReportTime > 1000) {
        Serial.print("Heart Rate: ");
        Serial.print(pox.getHeartRate());
        Serial.print(" bpm | SpO2: ");
        Serial.print(pox.getSpO2());
        Serial.println(" %");

        lastReportTime = millis();
    }
}