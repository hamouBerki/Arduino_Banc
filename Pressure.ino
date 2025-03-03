#include<Arduino.h>
#include<Wire.h>


uint8_t id = 0x28; //i2c address
uint8_t data[7]; // holds output data
uint8_t commandes[3] = {0xAA, 0x00, 0x00}; // command to be sent
double press_counts = 0; // digital pressure reading [counts]
double temp_counts = 0; // digital temperature reading [counts]
double pressure = 0; // pressure reading [bar, psi, kPa, etc.]
double temperature = 0; // temperature reading in deg C
double outputmax = 15099494; // output at maximum pressure [counts] 
double outputmin = 1677722; // output at minimum pressure [counts]
double pmax = 10; // maximum value of pressure range [bar, psi, kPa, etc.]
double pmin = 0; // minimum value of pressure range [bar, psi, kPa, etc.]
double percentage = 0; // holds percentage of full scale data
char printBuffer[200];
int peol=6;


void setup() {
//  pinMode(psensor, OUTPUT); //initializes digital pin 2 as an output
//  pinMode(pclock, OUTPUT);
  pinMode(peol, INPUT);  
  Serial.begin(9600);
  while(!Serial) {
    delay(10);
  }  
  Wire.begin();
  sprintf(printBuffer, "\nStatus Register, 24-bit Sensor data, Digital Pressure Counts, Percentage of full scale pressure, Pressure Output\n");
  Serial.println(printBuffer);
}
  
  void loop() {
  Wire.beginTransmission(id);
  int stat = Wire.write (commandes, 3); // write command to the sensor
  stat |= Wire.endTransmission();
  delay(10);
  Wire.requestFrom(id, 7); // read back Sensor data 7 bytes
  int i = 0;
  for (i = 0; i < 7; i++){ 
  data [i] = Wire.read();
  }  
  press_counts = data[3]+ data[2]*256+ data[1]*65536;// calculate digital pressure counts
  temp_counts = data[6]+ data[5]*256+ data[4]*65536;// calculate digital temperature counts
  temperature = (temp_counts *200/16777215)- 50; // calculate temperature in deg c    
  percentage = (press_counts /16777215)* 100; // calculate pressure as percentage of full scale  
  pressure =((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin; //calculation of pressure value according to equation 2 of datasheet    
  /*The below code prints the raw data as well as the processed data
  Data format : Status Register, 24-bit Sensor Data, Digital Counts, percentage of full scale pressure,
  pressure output, temperature */
  
  sprintf(printBuffer,"%x\t%2x %2x %2x\t%f\t%f\t%f\t%f \n",data[0], data[1], data[2], data[3], press_counts, percentage, pressure, temperature);  
  Serial.print(printBuffer);  
  delay(100);  
  }
  
