#include <IRremote.hpp>
#include <Keyboard.h>

#define IR_PIN 2

const byte buttonPins[] = {1,2,3,4,5,6,7,8};
bool lastState[8];

void setup() {

  Keyboard.begin();
  IrReceiver.begin(IR_PIN);

  for(int i=0;i<8;i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
    lastState[i] = HIGH;
  }
}

void loop() {

  // -------- BUTTONS --------
  for(int i=0;i<8;i++){

    bool state = digitalRead(buttonPins[i]);

    if(state == LOW && lastState[i] == HIGH){

      Keyboard.write('1' + i);
      delay(25);

    }

    lastState[i] = state;
  }


  // -------- IR REMOTE --------
  if(IrReceiver.decode()){

    uint32_t raw = IrReceiver.decodedIRData.decodedRawData;

    switch(raw){

      case 0x76A77416:
        Keyboard.write(KEY_RETURN);
        break;

      case 0x6212287:
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

      case 0xF574DC41:
        Keyboard.write('s');
        break;

      case 0x77A775AB:
        Keyboard.write(KEY_F11);   
        break;

      case 0x8453B5A5:
        Keyboard.write(KEY_F12);   
        break;
    }

    IrReceiver.resume();
  }
}
