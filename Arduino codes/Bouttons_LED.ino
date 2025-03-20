/*
  Ce programme utilise deux boutons pour contrôler une LED :
  - Bouton 1 (pin 2) allume la LED.
  - Bouton 2 (pin 3) éteint la LED.
*/

const int buttonOnPin = 2;   // Pin du bouton pour allumer la LED
const int buttonOffPin = 3;  // Pin du bouton pour éteindre la LED
const int ledPin = 13;       // Pin de la LED

int lastButtonOnState = HIGH;
int lastButtonOffState = HIGH;

unsigned long lastDebounceTimeOn = 0;
unsigned long lastDebounceTimeOff = 0;
const unsigned long debounceDelay = 50; // Délai anti-rebond

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonOnPin, INPUT_PULLUP);  // Utilisation des résistances pull-up internes
  pinMode(buttonOffPin, INPUT_PULLUP);
}

void loop() {
  int buttonOnState = digitalRead(buttonOnPin);
  int buttonOffState = digitalRead(buttonOffPin);

  // Gestion du debounce pour le bouton ON
  if (buttonOnState != lastButtonOnState) {
    lastDebounceTimeOn = millis();
  }

  if ((millis() - lastDebounceTimeOn) > debounceDelay) {
    if (buttonOnState == LOW) {
      digitalWrite(ledPin, HIGH); // Allumer la LED
    }
  }

  // Gestion du debounce pour le bouton OFF
  if (buttonOffState != lastButtonOffState) {
    lastDebounceTimeOff = millis();
  }

  if ((millis() - lastDebounceTimeOff) > debounceDelay) {
    if (buttonOffState == LOW) {
      digitalWrite(ledPin, LOW); // Éteindre la LED
    }
  }

  lastButtonOnState = buttonOnState;
  lastButtonOffState = buttonOffState;
}
