#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdint.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------- IR RX ----------
const int irRxPin = A2;

// ---------- IR TX (on receiver) ----------
const int irTxPin = A3;

// ---------- Timing ----------
const unsigned long bitTime = 50;
const unsigned long startThreshold = 150;
const unsigned long burstCycleDuration = 20000;

// ---------- State ----------
uint64_t receivedValue = 0;
bool numberReceived = false;

bool carrierDetected() {
  return digitalRead(irRxPin) == LOW;
}

void sendBurst() {
  tone(irTxPin, 38000);
  delay(50);
  noTone(irTxPin);
  digitalWrite(irTxPin, LOW);
}

uint64_t receiveNumber() {
  uint64_t value = 0;
  delay(bitTime);
  unsigned long bitStart = millis();
  for (int i = 0; i < 34; i++) {
    unsigned long base = bitStart + (unsigned long)i * 100;
    while (millis() < base + 20) {}
    int s1 = carrierDetected() ? 1 : 0;
    while (millis() < base + 25) {}
    int s2 = carrierDetected() ? 1 : 0;
    while (millis() < base + 30) {}
    int s3 = carrierDetected() ? 1 : 0;
    int bitVal = (s1 + s2 + s3) >= 2 ? 1 : 0;
    value = (value << 1) | (uint64_t)bitVal;
  }
  return value;
}

void printUint64(uint64_t value) {
  if (value == 0) {
    Serial.print("0");
    return;
  }
  char buf[21];
  int i = 20;
  buf[i] = '\0';
  while (value > 0) {
    i--;
    buf[i] = '0' + (value % 10);
    value /= 10;
  }
  Serial.print(&buf[i]);
}

void printUint64Binary(uint64_t value, int numBits) {
  for (int i = numBits - 1; i >= 0; i--) {
    Serial.print((char)('0' + ((value >> i) & 1ULL)));
  }
}

void printUint64LCD(uint64_t value) {
  if (value == 0) {
    lcd.print("0");
    return;
  }
  char buf[21];
  int i = 20;
  buf[i] = '\0';
  while (value > 0) {
    i--;
    buf[i] = '0' + (value % 10);
    value /= 10;
  }
  lcd.print(&buf[i]);
}

void setup() {
  pinMode(irRxPin, INPUT);
  pinMode(irTxPin, OUTPUT);
  digitalWrite(irTxPin, LOW);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Sending signal");

  // ---------- Burst cycle ----------
  unsigned long cycleStart = millis();
  while (millis() - cycleStart < burstCycleDuration) {
    sendBurst();
    delay(100);
  }

  // ---------- Done sending, now listen ----------
  lcd.clear();
  lcd.print("Waiting for");
  lcd.setCursor(0, 1);
  lcd.print("code...");
}

void loop() {
  if (numberReceived) return;

  if (!carrierDetected()) return;

  // Measure pulse duration
  unsigned long pulseStart = millis();
  while (carrierDetected()) {}
  unsigned long duration = millis() - pulseStart;

  if (duration >= startThreshold) {
    lcd.clear();
    lcd.print("Receiving...");
    receivedValue = receiveNumber();
    numberReceived = true;

    Serial.print("Received decimal: ");
    printUint64(receivedValue);
    Serial.println();
    Serial.print("Received binary:  ");
    printUint64Binary(receivedValue, 34);
    Serial.println();

    lcd.setCursor(0, 0);
    lcd.print("Received:       ");
    lcd.setCursor(0, 1);
    printUint64LCD(receivedValue);
  }
}