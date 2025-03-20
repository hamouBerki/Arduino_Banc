#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define TCA9548A_ADDR 0x70  // Adresse I2C du TCA9548A

// Définition des canaux
#define SENSOR1_CHANNEL 1    // Capteur de pression 1
#define SENSOR2_CHANNEL 2    // Capteur de pression 2
#define SENSOR3_CHANNEL 6    // MAX30100 - Capteur cardiaque 1
#define SENSOR4_CHANNEL 7    // MAX30100 - Capteur cardiaque 2

// Adresse I2C des capteurs de pression
uint8_t pressureSensorAddr = 0x28;
uint8_t data[7];
uint8_t command[3] = {0xAA, 0x00, 0x00};

// Constantes pour le calcul de la pression et de la température
double outputmax = 15099494;
double outputmin = 1677722;
double pmax = 10;
double pmin = 0;

// Objets pour les capteurs MAX30100
PulseOximeter pox1;
PulseOximeter pox2;
uint32_t lastReportTime = 0;

// Sélection du canal du multiplexeur
void selectI2CChannel(uint8_t channel) {
    if (channel > 7) return;
    Wire.beginTransmission(TCA9548A_ADDR);
    Wire.write(1 << channel);
    Wire.endTransmission();
    delay(10);
}

// Callback pour détection de battement
void onBeatDetected1() {
    Serial.println("Battement détecté sur le capteur 1!");
}

void onBeatDetected2() {
    Serial.println("Battement détecté sur le capteur 2!");
}

void capturePressure() {
    Wire.beginTransmission(pressureSensorAddr);
    Wire.write(command, 3);
    Wire.endTransmission();
    delay(10);
    Wire.requestFrom(pressureSensorAddr, 7);
    
    for (int i = 0; i < 7; i++) {
        data[i] = Wire.read();
    }
    
    double press_counts = data[3] + data[2] * 256 + data[1] * 65536;
    double temp_counts = data[6] + data[5] * 256 + data[4] * 65536;
    double temperature = (temp_counts * 200 / 16777215) - 50;
    double percentage = (press_counts / 16777215) * 100;
    double pressure = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;
    
    Serial.print("Pression: ");
    Serial.print(pressure);
    Serial.print(" | Température: ");
    Serial.println(temperature);
}

void setup() {
    Serial.begin(115200);
    Wire.begin();
    Serial.println("Initialisation des capteurs...");

    // Initialisation des MAX30100
    selectI2CChannel(SENSOR3_CHANNEL);
    if (!pox1.begin()) Serial.println("Erreur initialisation MAX30100 capteur 1!");
    pox1.setOnBeatDetectedCallback(onBeatDetected1);

    selectI2CChannel(SENSOR4_CHANNEL);
    if (!pox2.begin()) Serial.println("Erreur initialisation MAX30100 capteur 2!");
    pox2.setOnBeatDetectedCallback(onBeatDetected2);

    Serial.println("Capteurs initialisés!");
}

void loop() {
    // Lecture du capteur de pression 1
    selectI2CChannel(SENSOR1_CHANNEL);
    Serial.println("Lecture capteur de pression 1");
    capturePressure();

    // Lecture du capteur de pression 2
    selectI2CChannel(SENSOR2_CHANNEL);
    Serial.println("Lecture capteur de pression 2");
    capturePressure();

    // Lecture du capteur cardiaque 1
    selectI2CChannel(SENSOR3_CHANNEL);
    pox1.update();

    // Lecture du capteur cardiaque 2
    selectI2CChannel(SENSOR4_CHANNEL);
    pox2.update();

    // Affichage des résultats toutes les secondes
    if (millis() - lastReportTime > 1000) {
        Serial.print("Capteur 1 - HR: ");
        Serial.print(pox1.getHeartRate());
        Serial.print(" bpm | SpO2: ");
        Serial.print(pox1.getSpO2());
        Serial.println(" %");

        Serial.print("Capteur 2 - HR: ");
        Serial.print(pox2.getHeartRate());
        Serial.print(" bpm | SpO2: ");
        Serial.print(pox2.getSpO2());
        Serial.println(" %");

        lastReportTime = millis();
    }
}
