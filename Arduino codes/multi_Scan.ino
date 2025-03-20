#include <Wire.h>
#include "TCA9548A.h"  // Include the TCA9548A multiplexer library


/* ce code scann les channel une par une, et detecte si des capteur sont branchés et renvoie leurs addresses
        teste fait avec 2 capteurs cardiaques (detectées) un capteur pression  */


        /*adresse du capteur pression 0X28
        adresse MUX 0X70
        capteur MAX30100 0X57*/


TCA9548A I2CMux;  // Create an instance of the multiplexer

void scanI2CChannel(uint8_t channel) {
    I2CMux.openChannel(channel);  // Open the specific channel
    Serial.print("Scanning I2C devices on channel ");
    Serial.println(channel);

    bool deviceFound = false;
   
    for (uint8_t address = 1; address < 127; address++) { // Loop through all possible I2C addresses
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) {  // If the device responds
            Serial.print(" - Device found at address: 0x");
            Serial.println(address, HEX);
            deviceFound = true;
        }
    }

    if (!deviceFound) {
        Serial.println(" - No devices found on this channel.");
    }

    I2CMux.closeChannel(channel);  // Close the channel after scanning
    delay(100);
}

void setup() {
    Serial.begin(9600);
    while (!Serial); // Wait for Serial Monitor to open
    Wire.begin();   // Initialize I2C communication
    I2CMux.begin(Wire); // Initialize the multiplexer

    Serial.println("\nStarting I2C scan for TCA9548A channels...\n");

    for (uint8_t ch = 0; ch < 8; ch++) {  // Scan all 8 channels
        scanI2CChannel(ch);
        Serial.println("------------------------------");
    }

    Serial.println("\nScan complete.");
}

void loop() {
    // No need to repeat the scan unless you modify it for continuous scanning.
}
