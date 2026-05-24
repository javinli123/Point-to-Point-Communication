#include <Stepper.h>
#include <stdint.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------- Keypad ----------
int CharCount = 0;
const byte ROWS = 4;
const byte COLS = 4;
char Code[11];
int CodeConfirmed = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};
byte rowPins[ROWS] = {13, 12, 11, 10};
byte colPins[COLS] = {9, 8, 7, 6};
Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ---------- Stepper ----------
const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 2, 3, 4, 5);

// ---------- IR RX (on transmitter) ----------
const int irRxPin = A3;

// ---------- IR TX ----------
const int irTxPin = A2;
const unsigned int carrierFreq = 38000;

// ---------- Timing ----------
const unsigned long bitTime = 50;
const unsigned long startTime = 200;
const unsigned long burstCycleDuration = 20000;

// ---------- Sweep ----------
const int stepIncrement = 3;

// ---------- Message ----------
uint64_t numberToSend = 0;

int stepsForDegrees(float deg) {
  return (int)((deg / 360.0) * stepsPerRevolution);
}

bool checkSignal() {
  unsigned long start = millis();
  while (millis() - start < 30) {
    if (digitalRead(irRxPin) == LOW) return true;
  }
  return false;
}

void sendMark(unsigned long durationMs) {
  tone(irTxPin, carrierFreq);
  delay(durationMs);
  noTone(irTxPin);
}

void sendSpace(unsigned long durationMs) {
  noTone(irTxPin);
  delay(durationMs);
}

void sendBit(bool bitVal) {
  if (bitVal) {
    sendMark(bitTime);
  } else {
    sendSpace(bitTime);
  }
  sendSpace(50);
}

void sendNumber(uint64_t num) {
  sendMark(startTime);
  sendSpace(bitTime);
  for (int i = 33; i >= 0; i--) {
    bool bitVal = (num >> i) & 1ULL;
    sendBit(bitVal);
  }
  sendSpace(bitTime);
}

uint64_t codeToUint64(const char* code) {
  uint64_t result = 0;
  for (int i = 0; code[i] != '\0'; i++) {
    result = result * 10 + (code[i] - '0');
  }
  return result;
}

bool doSweep() {
  int steps50 = stepsForDegrees(50);
  int steps100 = stepsForDegrees(100);

  // Leg 1: left 50°
  for (int s = 0; s < steps50; s += stepIncrement) {
    myStepper.step(-stepIncrement);
    if (checkSignal()) return true;
  }

  // Leg 2: right 100° (through origin to +50°)
  for (int s = 0; s < steps100; s += stepIncrement) {
    myStepper.step(stepIncrement);
    if (checkSignal()) return true;
  }

  // Leg 3: left 100° (back through origin to -50°)
  for (int s = 0; s < steps100; s += stepIncrement) {
    myStepper.step(-stepIncrement);
    if (checkSignal()) return true;
  }

  // Leg 4: right 50° back to origin
  for (int s = 0; s < steps50; s += stepIncrement) {
    myStepper.step(stepIncrement);
    if (checkSignal()) return true;
  }

  return false;
}

void setup() {
  unsigned long powerOnTime = millis();  // record power on time

  Serial.begin(9600);
  pinMode(irRxPin, INPUT);
  pinMode(irTxPin, OUTPUT);
  noTone(irTxPin);
  myStepper.setSpeed(15);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  // ---------- Sweep ----------
  lcd.print("Searching...");

  bool found = doSweep();

  if (!found) {
    lcd.clear();
    lcd.print("No signal");
    lcd.setCursor(0, 1);
    lcd.print("Check alignment");
    while (true) {}  // halt, power cycle to retry
  }

  // ---------- Wait for receiver burst cycle to end ----------
  lcd.clear();
  lcd.print("Waiting...");
  unsigned long elapsed = millis() - powerOnTime;
  if (elapsed < burstCycleDuration) {
    delay(burstCycleDuration - elapsed);
  }

  // ---------- Keypad entry ----------
  lcd.clear();
  lcd.print("Enter 10 Nums");
  lcd.setCursor(0, 1);
  lcd.print("#:Del|*:Enter");
  delay(2000);
  lcd.clear();
  lcd.home();

  while (CodeConfirmed == 0) {
    char key = customKeypad.getKey();

    if (key != NO_KEY && key != '*' && key != '#' && key != 'A' && key != 'B' && key != 'C' && key != 'D') {
      if (CharCount < 10) {
        Code[CharCount] = key;
        CharCount++;
        Code[CharCount] = '\0';
        lcd.clear();
        lcd.print(Code);
        Serial.println(Code);
      }
    }

    if (key != NO_KEY && key == '*') {
      Code[0] = '\0';
      CharCount = 0;
      lcd.setCursor(0, 0);
      lcd.print("Code Deleted");
      delay(1000);
      lcd.clear();
      lcd.home();
    }

    if (key != NO_KEY && key == '#') {
      CodeConfirmed = 1;
      lcd.setCursor(0, 1);
      lcd.print("Code Confirmed");
      delay(1000);
    }

    if (CharCount == 10) {
      CodeConfirmed = 1;
      lcd.setCursor(0, 1);
      lcd.print("Auto Confirmed");
      delay(1000);
    }
  }

  numberToSend = codeToUint64(Code);

  // ---------- Send number ----------
  lcd.clear();
  lcd.print("Sending...");
  sendNumber(numberToSend);
  lcd.clear();
  lcd.print("Done!");
}

void loop() {
  // intentionally empty
}