volatile int NbTopsFan; //measuring the rising edges of the signal
float Calc;
int hallsensor = 2;    //The pin location of the sensor
 
void rpm ()     //This is the function that the interupt calls
{
    NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
}
// The setup() method runs once, when the sketch starts
void setup() //
{
    attachInterrupt(digitalPinToInterrupt(2), rpm, RISING);; //initializes digital pin 2 as an input
    Serial.begin(9600); //This is the setup function where the serial port is initialised,
}
// the loop() method runs over and over again,
// as long as the Arduino has power
void loop ()
{
  NbTopsFan = 0;   //Set NbTops to 0 ready for calculations
  interrupts();      //Enables interrupts
  delay(200);
  noInterrupts();      //Enables interrupts
  Calc = (NbTopsFan * 12 * 1.82 / 45); //(Pulse frequency x 60) / 7.5Q, = flow rate in L/min
  Serial.print (Calc); //Prints the number calculated above
  Serial.print (" L/min\r\n"); //Prints "L/hour" and returns a  new line
}
