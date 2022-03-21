/**
   Set the default values
*/
void defaultValues() {
  // Left Switch
  data.s1 = 1;

  // Left Pot
  data.p1 = 127;

  // Left Joystick
  data.j1u = 0; // Up
  data.j1d = 0; // Down
  data.j1l = 0; // Left
  data.j1r = 0; // Right
  data.j1b = 1; // Button

  // Left Thumb Joystick
  data.tj1u = 0; // Up
  data.tj1d = 0; // Down
  data.tj1l = 0; // Left
  data.tj1r = 0; // Right

  // Left Btn
  data.b1 = 1;

  // Right Switch
  data.s2 = 1;

  // Right Pot
  data.p2 = 127;

  // Right Joystick
  data.j2u = 0; // Up
  data.j2d = 0; // Down
  data.j2l = 0; // Left
  data.j2r = 0; // Right
  data.j2b = 1; // Button

  // Right Thumb Joystick
  data.tj2u = 0; // Up
  data.tj2d = 0; // Down
  data.tj2l = 0; // Left
  data.tj2r = 0; // Right

  // Right Btn
  data.b2 = 1;

  // Rotary Encoder
  // data.re;

  // Rotary Encoder - Push
  data.rep = 1;

  // Green Btn
  data.gb = 1;

  // Red Btn
  data.rb = 1;

  // Transmitter Time
  data.transmitterTime = millis();

  // Joystick Read
  joystick.val1 = 0;
  joystick.val2 = 0;

  // RX & TX Status
  RX_STATUS = false;
  TX_STATUS = false;

}

void defaultValuesRx() {
  //RX
  dataRx.voltage = 0;
}

/**
   Add extra digits to the number
*/
String addDigits(int val1, int val2, int width) {
  int len = 0;
  int value;
  if (val1 > 0 ) {
    value = val1;
  } else {
    value = val2;
  }

  String newString;
  String zero = "0";

  if (value > 9999)
    len = 5;
  else if (value > 999)
    len = 4;
  else if (value > 99)
    len = 3;
  else if (value > 9)
    len = 2;
  else
    len = 1;

  if (len < width) {
    len = width - len;
    while (len--) {
      newString += zero;
    }
  }

  newString += value;
  return newString;
}

/**
   Value round
*/
float valRound(float val) {
  return round(val * 100.0) / 100.0;
}


/**
   Read the joystick values and allocate the up and down / left and right
*/
int joystickRead(float value, int diff) {
  joystick.val1 = 0;
  joystick.val2 = 0;
  if (map(value, 0, 1023, 0, 255) > (127 + diff)) {
    joystick.val1 = map(value, 511, 1023, 0, 255);
    joystick.val2 = 0;
  } else if (map(value, 0, 1023, 0, 255) < (127 - diff)) {
    joystick.val1 = 0;
    joystick.val2 = map(value, 511, 0, 0, 255);
  }
}
