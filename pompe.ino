#define POMPE_PIN 9  // Broche PWM connectée à la gate du MOSFET

/* ce code est concus pour tester la pompe sous differentes vitesses (utiliser l'alim HAMEG)
        Rq : si la pompe ne fonctionne pas comme attendu vérifier la connectique, puis les composants 
        en dernier recours suivre le signal PWM avec un oscillo pour reperer où se situe le probleme
*/


void setup() {
  pinMode(POMPE_PIN, OUTPUT);
}

void loop() {
  Serial.println("tourner");
  analogWrite(POMPE_PIN, 150);  // max :255
  delay(3000);




  Serial.println("finis de tourner");
  analogWrite(POMPE_PIN, 0);    
  delay(3000);
}
