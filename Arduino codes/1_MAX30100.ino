#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define TCA9548A_ADDR 0x70  // I2C Address of TCA9548A
#define SENSOR1_CHANNEL 1    // First MAX30100
#define SENSOR2_CHANNEL 4    // Second MAX30100

PulseOximeter pox1;
PulseOximeter pox2;
uint32_t lastReportTime = 0;

// Function to select the specific channel on TCA9548A
void selectI2CChannel(uint8_t channel) {
    if (channel > 7) return; // TCA9548A supports channels 0-7
    Wire.beginTransmission(TCA9548A_ADDR);
    Wire.write(1 << channel);
    Wire.endTransmission();
    delay(10); // Small delay to allow switching
}

// Callback function for beat detection (optional)
void onBeatDetected1() {
    Serial.println("♥ Beat Detected on Sensor 1!");
}

void onBeatDetected2() {
    Serial.println("♥ Beat Detected on Sensor 2!");
}

void setup() {
    Serial.begin(115200);
    Wire.begin(); // Initialize I2C

    Serial.println("Initializing Sensors...");

    // Select channel for first MAX30100
    selectI2CChannel(SENSOR1_CHANNEL);
    if (!pox1.begin()) {
        Serial.println("MAX30100 Sensor 1 initialization failed!");
    }
    pox1.setOnBeatDetectedCallback(onBeatDetected1);

    // Select channel for second MAX30100
    selectI2CChannel(SENSOR2_CHANNEL);
    if (!pox2.begin()) {
        Serial.println("MAX30100 Sensor 2 initialization failed!");
    }
    pox2.setOnBeatDetectedCallback(onBeatDetected2);

    Serial.println("Both MAX30100 Sensors Initialized!");
}

void loop() {
    // Read from First MAX30100 (Channel 7)
    selectI2CChannel(SENSOR1_CHANNEL);
    pox1.update();

    // Read from Second MAX30100 (Channel 6)
    selectI2CChannel(SENSOR2_CHANNEL);
    pox2.update();

    // Print data every second
    if (millis() - lastReportTime > 1000) {
        Serial.print("Sensor 1 - Heart Rate: ");
        Serial.print(pox1.getHeartRate());
        Serial.print(" bpm | SpO2: ");
        Serial.print(pox1.getSpO2());
        Serial.println(" %");

        Serial.print("Sensor 2 - Heart Rate: ");
        Serial.print(pox2.getHeartRate());
        Serial.print(" bpm | SpO2: ");
        Serial.print(pox2.getSpO2());
        Serial.println(" %");

        lastReportTime = millis();
    }
}