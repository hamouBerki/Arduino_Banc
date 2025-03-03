#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define TCA9548A_ADDRESS 0x70  // Adresse I2C du TCA9548A
#define CHANNEL_PRESSURE_SENSOR 7  // Canal du capteur de pression
#define CHANNEL_MAX30100 0     // Canal du capteur MAX30100
#define REPORTING_PERIOD_MS 1000

PulseOximeter pox;
uint32_t tsLastReport = 0;
bool isPressureSensor = false;

void tca9548a_select(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCA9548A_ADDRESS);
  Wire.write(1 << channel);
  Wire.endTransmission();
  Serial.print("Canal activé : ");
  Serial.println(channel);
}

uint8_t detectSensor(uint8_t channel) {
  tca9548a_select(channel);
  delay(10);

  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("Canal ");
      Serial.print(channel);
      Serial.print(" - Capteur détecté à l'adresse 0x");
      Serial.println(address, HEX);
      return address;
    }
  }
  return 0;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.print("Vérification du TCA9548A à l'adresse 0x70...");
  Wire.beginTransmission(TCA9548A_ADDRESS);
  if (Wire.endTransmission() == 0) {
    Serial.println(" OK !");
  } else {
    Serial.println(" NON DETECTE !");
  }

  Serial.println("Activation du canal 0...");
  tca9548a_select(0);
  delay(1000);
  detectSensor(CHANNEL_MAX30100);

  Serial.println("Activation du canal 7...");
  tca9548a_select(7);
  delay(1000);
  detectSensor(CHANNEL_PRESSURE_SENSOR);
}

void loop() {
  delay(5000);
  detectSensor(CHANNEL_MAX30100);
  detectSensor(CHANNEL_PRESSURE_SENSOR);
}
