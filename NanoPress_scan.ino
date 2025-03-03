#include <Wire.h>

void setup() {

  Serial.begin(9600);
  Wire.begin();
  Serial.println("Scann I2C running...");

}

void loop() {

  for (uint8_t add=1; add<127; add++) {
    Wire.beginTransmission(add);
    if(Wire.endTransmission() ==0){
      Serial.print("capteur detecter a l adresse : 0x");
      Serial.println(add, HEX);
    }
  }
    Serial.println("Scan termine");
    delay(500);
}