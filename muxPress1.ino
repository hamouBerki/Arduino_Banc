#include <Arduino.h>
#include <Wire.h>

                /*Code pour 1 capteur pression connecté a ch2 du MUX TCA9548A*/

#define TCA9548A_ADDR 0x70  // I2C address of TCA9548A multiplexer
#define SENSOR_ADDR 0x28     // I2C address of the pressure sensor

uint8_t data[7];  // Holds sensor output data
uint8_t commandes[3] = {0xAA, 0x00, 0x00}; // Command to send
double press_counts = 0;
double temp_counts = 0;
double pressure = 0;
double temperature = 0;
double outputmax = 15099494;
double outputmin = 1677722;
double pmax = 10;
double pmin = 0;
double percentage = 0;
char printBuffer[200];
int peol = 6;

// Function to select TCA9548A channel
void selectMuxChannel(uint8_t channel) {
  if (channel > 7) return; // le MUX possede 8 canneaux
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(1 << channel);  // Select channel by writing bit-shifted value
  Wire.endTransmission();
}

void setup() {
  pinMode(peol, INPUT);
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }
  
  Wire.begin();

  Serial.println("\nStatus Register, 24-bit Sensor Data, Digital Pressure Counts, Percentage of Full Scale Pressure, Pressure Output");

  // Select the MUX channel 
  selectMuxChannel(2);
}

void readSensor() {
  Wire.beginTransmission(SENSOR_ADDR);
  int stat = Wire.write(commandes, 3); // Write command to sensor
  stat |= Wire.endTransmission();
  delay(10);
  
  Wire.requestFrom(SENSOR_ADDR, 7); // Read sensor data
  for (int i = 0; i < 7; i++) { 
    data[i] = Wire.read();
  }  

  press_counts = data[3] + data[2] * 256 + data[1] * 65536; // digital pressure
  temp_counts = data[6] + data[5] * 256 + data[4] * 65536;  //digital temp
  temperature = (temp_counts * 200 / 16777215) - 50; // temp en C
  percentage = (press_counts / 16777215.0) * 100; //. % de la pression 
  pressure = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin; // press en unité physique

  // Print formatted data
  sprintf(printBuffer, "%x\t%2x %2x %2x\t%f\t%f\t%f\t%f \n", 
          data[0], data[1], data[2], data[3], press_counts, percentage, pressure, temperature);
  Serial.print(printBuffer);
}

void loop() {
  Serial.println("Reading from sensor on MUX Channel 2...");
  readSensor();  // Read the pressure sensor on channel 2
  delay(500);  // Adjust as needed
}
