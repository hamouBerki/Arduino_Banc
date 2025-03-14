#include <Arduino.h>
#include <Wire.h>

#define TCA9548A_ADDR 0x70  // Adresse I2C du multiplexeur TCA9548A
#define SENSOR_ADDR 0x28     // Adresse I2C des capteurs de pression (identique pour les deux)

uint8_t data[7];  // Stocke les données du capteur
uint8_t commandes[3] = {0xAA, 0x00, 0x00}; // Commande envoyée au capteur
double press_counts = 0;
double temp_counts = 0;
double pressure = 0;
double temperature = 0;
double outputmax = 15099494;
double outputmin = 1677722;
double pmax = 10; // Modifier selon l'unité de votre capteur (bar, psi, kPa)
double pmin = 0;
double percentage = 0;
char printBuffer[200];
int peol = 6;

// Fonction pour sélectionner le canal du multiplexeur
void selectMuxChannel(uint8_t channel) {
  if (channel > 7) return; // Le TCA9548A a 8 canaux (0-7)
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(1 << channel);  // Sélection du canal avec un décalage de bit
  Wire.endTransmission();
}

// Fonction pour lire les données d'un capteur
void readSensor(uint8_t channel) {
  selectMuxChannel(channel);  // Sélection du canal sur le TCA9548A
  
  Wire.beginTransmission(SENSOR_ADDR);
  int stat = Wire.write(commandes, 3); // Envoi de la commande au capteur
  stat |= Wire.endTransmission();
  delay(10);
  
  Wire.requestFrom(SENSOR_ADDR, 7); // Lecture des 7 octets de données
  for (int i = 0; i < 7; i++) { 
    data[i] = Wire.read();
  }  

  press_counts = data[3] + data[2] * 256 + data[1] * 65536;
  temp_counts = data[6] + data[5] * 256 + data[4] * 65536;
  temperature = (temp_counts * 200 / 16777215) - 50;
  percentage = (press_counts / 16777215.0) * 100;
  pressure = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;

  // Affichage des données du capteur
  Serial.print("Capteur sur le canal ");
  Serial.print(channel);
  Serial.print(": Pression = ");
  Serial.print(pressure, 3);
  Serial.print(" unités | Température = ");
  Serial.print(temperature, 2);
  Serial.println(" °C");
}

void setup() {
  pinMode(peol, INPUT);
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }
  
  Wire.begin();
  Serial.println("\nLecture des deux capteurs de pression via le TCA9548A...");
}

void loop() {
  readSensor(2); // Lire le capteur sur le canal 2
  delay(500);
  
  readSensor(5); // Lire le capteur sur le canal 5
  delay(500);
}
