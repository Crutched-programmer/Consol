#include <HID-Project.h>
#include <HID-Settings.h>
#include <IRremote.hpp>

#define IR_PIN 2

// number buttons
const int numButtons[] = {3,4,5,6,7,8,9,10,11};
bool lastNumState[9];

// macro buttons
const int focusPin = A3;
const int distractPin = A4;
const int silentPin = A5;

bool lastFocus = HIGH;
bool lastDistract = HIGH;
bool lastSilent = HIGH;

// IR debounce
uint32_t lastIRCode = 0;
unsigned long lastIRTime = 0;
const int IR_DEBOUNCE = 100; // ms

void setup() {

  Keyboard.begin();
  Consumer.begin();

  IrReceiver.begin(IR_PIN);

  for(int i=0;i<9;i++){
    pinMode(numButtons[i], INPUT_PULLUP);
    lastNumState[i] = HIGH;
  }

  pinMode(focusPin, INPUT_PULLUP);
  pinMode(distractPin, INPUT_PULLUP);
  pinMode(silentPin, INPUT_PULLUP);
}

void loop() {

  // NUMBER BUTTONS
  for(int i=0;i<9;i++){

    bool state = digitalRead(numButtons[i]);

    if(state == LOW && lastNumState[i] == HIGH){

      Keyboard.write('1' + i);
      delay(25);
    }

    lastNumState[i] = state;
  }


  // FOCUS
  bool focusState = digitalRead(focusPin);

  if(focusState == LOW && lastFocus == HIGH){

    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(100);
    Keyboard.releaseAll();

    delay(1000);

    Keyboard.print("cmd");
    Keyboard.write(KEY_ENTER);

    delay(2000);

    Keyboard.print("start https://trigger.macrodroid.com/610c364c-5923-4cbb-b4b3-da753b6e9fe9/focuuuss");
    Keyboard.write(KEY_ENTER);

    delay(300);
  }

  lastFocus = focusState;


  // DISTRACT
  bool distractState = digitalRead(distractPin);

  if(distractState == LOW && lastDistract == HIGH){

    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(100);
    Keyboard.releaseAll();

    delay(1000);

    Keyboard.print("cmd");
    Keyboard.write(KEY_ENTER);

    delay(2000);

    Keyboard.print("start https://trigger.macrodroid.com/610c364c-5923-4cbb-b4b3-da753b6e9fe9/distract");
    Keyboard.write(KEY_ENTER);

    delay(300);
  }

  lastDistract = distractState;


  // SILENT
  bool silentState = digitalRead(silentPin);

  if(silentState == LOW && lastSilent == HIGH){

    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(100);
    Keyboard.releaseAll();

    delay(1000);

    Keyboard.print("cmd");
    Keyboard.write(KEY_ENTER);

    delay(2000);

    Keyboard.print("start https://trigger.macrodroid.com/610c364c-5923-4cbb-b4b3-da753b6e9fe9/silent");
    Keyboard.write(KEY_ENTER);

    delay(300);
  }

  lastSilent = silentState;


  // IR RECEIVER WITH DEBOUNCE
  if(IrReceiver.decode()){

    uint32_t code = IrReceiver.decodedIRData.decodedRawData;
    unsigned long now = millis();

    // ignore repeated signals within debounce window
    if(code != lastIRCode || (now - lastIRTime) > IR_DEBOUNCE){

      lastIRCode = code;
      lastIRTime = now;

      switch(code){

        case 0x76A77416:
          Keyboard.write(KEY_ENTER);
          break;

        case 0x06212287:
          Keyboard.write(KEY_UP_ARROW);
          break;

        case 0xDE3AA631:
          Keyboard.write(KEY_DOWN_ARROW);
          break;

        case 0x8753BA5C:
          Keyboard.write(KEY_LEFT_ARROW);
          break;

        case 0x69893291:
          Keyboard.write(KEY_RIGHT_ARROW);
          break;

        case 0xEB58E7B6:
          Keyboard.write(KEY_ESC);
          break;

        case 0x77A775AB:
          Consumer.write(MEDIA_VOLUME_UP);
          break;

        case 0x8453B5A5:
          Consumer.write(MEDIA_VOLUME_DOWN);
          break;
      }
    }

    IrReceiver.resume();
  }
}
