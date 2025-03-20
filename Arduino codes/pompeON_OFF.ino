#define POMPE_PIN 9  // Broche PWM connectée à la gate du MOSFET
#define BUTTON_UP 2   // Bouton pour augmenter le PWM
#define BUTTON_DOWN 3 // Bouton pour diminuer le PWM

const int pwmMax = 255;
const int pwmMin = 0;
const int pwmStep = 25; // Incrément/décrément de PWM
const unsigned long debounceDelay = 50; // Délai anti-rebond

int pwmValue = 0; // Valeur actuelle du signal PWM
int lastButtonUpState = HIGH;
int lastButtonDownState = HIGH;

unsigned long lastDebounceTimeUp = 0;
unsigned long lastDebounceTimeDown = 0;

void setup() {
  pinMode(POMPE_PIN, OUTPUT);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  int buttonUpState = digitalRead(BUTTON_UP);
  int buttonDownState = digitalRead(BUTTON_DOWN);

  // Gestion du debounce pour le bouton d'augmentation
  if (buttonUpState != lastButtonUpState) {
    lastDebounceTimeUp = millis();
  }

  if ((millis() - lastDebounceTimeUp) > debounceDelay) {
    if (buttonUpState == LOW) {
      pwmValue += pwmStep;
      if (pwmValue > pwmMax) pwmValue = pwmMax;
      analogWrite(POMPE_PIN, pwmValue);
      Serial.print("Augmentation PWM: ");
      Serial.println(pwmValue);
    }
  }

  // Gestion du debounce pour le bouton de diminution
  if (buttonDownState != lastButtonDownState) {
    lastDebounceTimeDown = millis();
  }

  if ((millis() - lastDebounceTimeDown) > debounceDelay) {
    if (buttonDownState == LOW) {
      pwmValue -= pwmStep;
      if (pwmValue < pwmMin) pwmValue = pwmMin;
      analogWrite(POMPE_PIN, pwmValue);
      Serial.print("Diminution PWM: ");
      Serial.println(pwmValue);
    }
  }

  lastButtonUpState = buttonUpState;
  lastButtonDownState = buttonDownState;
}
