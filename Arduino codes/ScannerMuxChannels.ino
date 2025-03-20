#include <Wire.h>

/*Code pour detecter uniquement des capteurs avc la meme @*/

#define TCA9548A_ADDRESS 0x70  // Adresse du multiplexeur I2C

// Fonction pour sélectionner un canal du MUX
void tca9548a_select(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCA9548A_ADDRESS);
  Wire.write(1 << channel);  // Active uniquement le canal sélectionné
  Wire.endTransmission();
}

// Fonction pour scanner le bus I2C
void scanI2C() {
  Serial.println("Scanning I2C bus...");

  for (uint8_t channel = 0; channel < 8; channel++) {
    tca9548a_select(channel);  // Active le canal du MUX
    Serial.print("Canal ");
    Serial.print(channel);
    Serial.println(":");

    for (uint8_t address = 1; address < 127; address++) {
      Wire.beginTransmission(address);
      if (Wire.endTransmission() == 0) {  // Si un périphérique répond
        Serial.print("  → Détecté à l'adresse 0x");
        Serial.println(address, HEX);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);
  scanI2C();  // Lancer le scan I2C au démarrage
}

void loop() {
}
