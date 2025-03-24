          /* Ce code est réaliser pour controler la vitesse d'une seul pompe avec 2 boutton avant de lancé 
                        le code assurer vous que votre connectique est correcte
              conseil : essayer d'abord d'allumer/eteindre une led avec le code sur Git*/


/* Ce code inclus aussi une partie affichae LCD */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Adresse I2C de l'écran LCD (souvent 0x27 ou 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);





#define POMPE_PIN 9  // Broche PWM connectée à la gate du MOSFET
#define BUTTON_UP 2   // Bouton pour augmenter le PWM
#define BUTTON_DOWN 3 // Bouton pour diminuer le PWM

const int pwmMax = 255; /* voir le cahier pour l'explication*/
const int pwmMin = 0;
const int pwmStep = 25; // Incrément/décrément de PWM
const unsigned long debounceDelay = 20; // Délai anti-rebond


      /*Le rebond s'est le changement d'état non volontaire du boutton par influance mécanique
                (un effet ressort qui est consideré comme un appuis du boutton) */

int pwmValue = 0; // le systeme commence par PWM = 0
int lastButtonUpState = HIGH;
int lastButtonDownState = HIGH;  //mémorise l'état du boutton pour détecter les changements 

unsigned long lastDebounceTimeUp = 0;
unsigned long lastDebounceTimeDown = 0; 
bool buttonUpPressed = false;
bool buttonDownPressed = false;

void setup() {
  pinMode(POMPE_PIN, OUTPUT);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Pump under controle");
  delay(1000);
  lcd.clear();
  afficherVitesse();
  Serial.println("boutton haut --> augmenter/n/nbouttont du bas --> diminuer.");
}

// Fonction de gestion du bouton avec debounce
bool buttonPressed(int buttonPin, int &lastButtonState, unsigned long &lastDebounceTime, bool &buttonHandled) {
  int buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) { //millis() compte le temps depuis le moment ou l'arduino est lancé
    lastDebounceTime = millis(); //on mémorise le moment du changement
  }

  if ((millis() - lastDebounceTime) > debounceDelay) { //si le temps écoulé depuis le dernier changement > delay de changement (état stable)
    if (buttonState == LOW && !buttonHandled) {
      buttonHandled = true;
      lastButtonState = buttonState;
      return true;
    }
    if (buttonState == HIGH) {
      buttonHandled = false;
    }
  }

  lastButtonState = buttonState;
  return false;
}

// Fonction pour afficher la vitesse de la pompe
void afficherVitesse() {
  float pourcentage = (pwmValue / 255.0) * 100.0;

  //ajout LCD
  Serial.print("Vitesse de la pompe : ");
  Serial.print(pourcentage, 1);
  Serial.println("%");
  lcd.clear();
  lcd.setCursor(0, 0);
  //fin de l'ajout

  Serial.print("Vitesse de la pompe: ");
  Serial.print(pourcentage, 1); // Affichage avec une décimale
  Serial.println("%");
}

void loop() {
  if (buttonPressed(BUTTON_UP, lastButtonUpState, lastDebounceTimeUp, buttonUpPressed)) {
    pwmValue += pwmStep;
    if (pwmValue > pwmMax) pwmValue = pwmMax;
    analogWrite(POMPE_PIN, pwmValue);
    Serial.println("Augmentation de la vitesse.");
    afficherVitesse();
  }

  if (buttonPressed(BUTTON_DOWN, lastButtonDownState, lastDebounceTimeDown, buttonDownPressed)) {
    pwmValue -= pwmStep;
    if (pwmValue < pwmMin) pwmValue = pwmMin;
    analogWrite(POMPE_PIN, pwmValue);
    Serial.println("Diminution de la vitesse.");
    afficherVitesse();
  }
}
