#include <Encoder.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

using namespace Phantom;

Encoder enc(2, 3);
LiquidCrystal_I2C lcd(0x3F, 16, 4);

uint8_t lastEncoder = 1;

const uint16_t PITCH_LOW = 262;
const uint16_t PITCH_HIGH = 523;
uint8_t toneStep = 0;

void setup() {
  // LCD Setup
  lcd.init();
  lcd.backlight();

  // Encoder Setup
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(INT0, encoderUpdate, CHANGE);
  attachInterrupt(INT1, encoderUpdate, CHANGE);

  // Serial Setup
  Serial.begin(57600);
}

void loop() {
  // Print BPM Value
  uint8_t currentEncoder = enc.get() / 4;
  if (lastEncoder != currentEncoder) {
    lcd.clear();
    lcd.print(currentEncoder);
    lcd.print("BPM");
    lastEncoder = currentEncoder;
  }

  if (millis() % (15000 / currentEncoder) == 0) { // This uses polling which is bad if the uC is loaded with tasks
    
    if (toneStep == 0) {
      tone(5, 1047, 150);
    }
    else {
      tone(5, PITCH_LOW, 150);
    }

    delay(200);

    toneStep += 1;
  }
  if (toneStep == 4) {
    toneStep = 0;
  }
}

void encoderUpdate() {
  enc.update();
}

