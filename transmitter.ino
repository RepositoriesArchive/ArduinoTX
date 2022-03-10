/**
  Libraries
*/
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h> // Required for "radio.printDetails();"
#include <MCP3XXX.h>
#include <Adafruit_GFX.h>       // include Adafruit graphics library
#include <Adafruit_ILI9341.h>   // include Adafruit ILI9341 TFT library

bool debug = true;

MCP3008 adc;

/**
   MCP3008 pin connections
*/
#define CS_PIN 4
#define CLOCK_PIN 7
#define MOSI_PIN 5
#define MISO_PIN 6

/**
   Buttons pin connections
*/
// Left Switch
#define leftSwitch 8

// Left Pot
#define leftPot 6

// Left Joystick
#define leftJoystickUpDown 24
#define leftJoystickLeftRight 25
#define leftJoystickButton 21

// Left Thumb Joystick
#define leftThumbUpDown 0
#define leftThumbLeftRight 1

// Left Button
#define leftButton 17

// Right Switch
#define rightSwitch 9

// Right Pot
#define rightPot 4

// Right Joystick
#define rightJoystickUpDown 30
#define rightJoystickLeftRight 31
#define rightJoystickButton 16

// Right Thumb Joystick
#define rightThumbUpDown 2
#define rightThumbLeftRight 3

// Right Btn
#define rightButton 15

// Rotary Encoder
#define rotaryEncoderLeft 19
#define rotaryEncoderRight 20
#define rotaryEncoderPush 18

// Button 1 (green)
#define button1 10

// Button 2 (red)
#define button2 11

/**
   TFT
*/
#define TFT_RST   26
#define TFT_DC    0
#define TFT_CS    1
#define TFT_LCD   12

// initialize ILI9341 TFT library
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
/** -------- */

/**
   nRF24L01
*/
#define RX_CE 13
#define RX_CSN 14
#define RX_CHANNEL 108  // 2.508 Ghz - Above most Wifi Channels

#define TX_CE 23
#define TX_CSN 22
#define TX_CHANNEL 108  // 2.508 Ghz - Above most Wifi Channels

//TX Setup
const byte address[6] = "00001";
const char text[] = "Hello World";
char textRx[32] = "";

RF24 radioTx(TX_CE, TX_CSN);
RF24 radioRx(RX_CE, RX_CSN);
/** -------- */

float voltage;

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

// Joystick Read
struct joystickData {
  byte val1;
  byte val2;
};
joystickData joystick;

// Store the current values
struct package {
  // Left Switch
  byte s1;

  // Left Pot
  byte p1;

  // Left Joystick
  byte j1u; // Up
  byte j1d; // Down
  byte j1l; // Left
  byte j1r; // Right
  byte j1b; // Button

  // Left Thumb Joystick
  byte tj1u; // Up
  byte tj1d; // Down
  byte tj1l; // Left
  byte tj1r; // Right

  // Left Btn
  byte b1;

  // Right Switch
  byte s2;

  // Right Pot
  byte p2;

  // Right Joystick
  byte j2u; // Up
  byte j2d; // Down
  byte j2l; // Left
  byte j2r; // Right
  byte j2b; // Button

  // Right Thumb Joystick
  byte tj2u; // Up
  byte tj2d; // Down
  byte tj2l; // Left
  byte tj2r; // Right

  // Right Btn
  byte b2;

  // Rotary Encoder
  byte re;

  // Rotary Encoder - Push
  byte rep;

  // Green Btn
  byte gb;

  // Red Btn
  byte rb;

  // Transmitter Time
  byte transmitterTime;
};

// Create a variable with the above structure
package data;
package dataRx;

void setup() {
  if (debug) {
    /* Serial Port */
    Serial.begin(57600);
  }

  /**
     Activate the Arduino internal pull-up resistors
  */
  pinMode(8, INPUT);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);

  // MCP3008 Init
  adc.begin();

  // TFT INIT
  pinMode(TFT_LCD, OUTPUT);
  analogWrite(TFT_LCD, 5);
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(45);
  testText();

  /**
    radioTx Init
  */
  radioTx.begin();
  radioTx.setPALevel(RF24_PA_MIN);  // Reduce Power Consumption
  radioTx.setDataRate(RF24_250KBPS);  // Reduce Power Consumption
  radioTx.setChannel(TX_CHANNEL);
  radioTx.openWritingPipe(address);
  radioTx.stopListening();

  if (debug) {
    printf_begin();
    radioTx.printDetails();
    Serial.print("TX Connection Status: ");
    Serial.println(radioTx.isChipConnected());
  }

  /**
     radioRx Set-Up
  */
  radioRx.begin();
  radioRx.setPALevel(RF24_PA_MIN);  // Reduce Power Consumption
  radioRx.setDataRate(RF24_250KBPS);  // Reduce Power Consumption
  radioRx.setChannel(RX_CHANNEL);
  radioRx.openReadingPipe(0, address);
  radioRx.startListening();

  if (debug) {
    printf_begin();
    radioRx.printDetails();
    Serial.print("RX Connection Status: ");
    Serial.println(radioRx.isChipConnected());
  }

  /**
     Set the default values
  */
  defaultValues();
}


/**
   Read the joystick values and allocate the up and down
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

void loop() {

  /* Transmission Message */
  if (radioRx.available()) {
    radioRx.read(&dataRx, sizeof(package));
    /**
       If data received, update the lastReceiveTime
    */
    lastReceiveTime = millis();

    // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    if ( currentTime - lastReceiveTime > 1000 ) {
      /**
         Either use the default data or adjust to some safe values based on your device
      */
    }
    if (debug) {
      Serial.print("s1: ");
      Serial.print(data.s1);
      Serial.print(";");

      Serial.print("p1: ");
      Serial.print(data.p1);
      Serial.print(";");

      Serial.print("j1ud: ");
      Serial.print(data.j1u);
      Serial.print(":");
      Serial.print(data.j1d);
      Serial.print(";");

      Serial.print("j1lr: ");
      Serial.print(data.j1l);
      Serial.print(":");
      Serial.print(data.j1r);
      Serial.print(";");

      Serial.print("j1b: ");
      Serial.print(data.j1b);
      Serial.print(";");

      Serial.print("tj1ud: ");
      Serial.print(data.tj1u);
      Serial.print(":");
      Serial.print(data.tj1d);
      Serial.print(";");

      Serial.print("tj1lr: ");
      Serial.print(data.tj1l);
      Serial.print(":");
      Serial.print(data.tj1r);
      Serial.print(";");

      Serial.print("b1: ");
      Serial.print(data.b1);
      Serial.print(";");

      Serial.print("s2: ");
      Serial.print(data.s2);
      Serial.print(";");

      Serial.print("p2: ");
      Serial.print(data.p2);
      Serial.print(";");

      Serial.print("j2u: ");
      Serial.print(data.j2u);
      Serial.print(":");
      Serial.print(data.j2d);
      Serial.print(";");

      Serial.print("j2l: ");
      Serial.print(data.j2l);
      Serial.print(":");
      Serial.print(data.j2r);
      Serial.print(";");

      Serial.print("j2b: ");
      Serial.print(data.j2b);
      Serial.print(";");

      Serial.print("tj2u: ");
      Serial.print(data.tj2u);
      Serial.print(":");
      Serial.print(data.tj2d);
      Serial.print(";");

      Serial.print("tj2l: ");
      Serial.print(data.tj2l);
      Serial.print(":");
      Serial.print(data.tj2r);
      Serial.print(";");

      Serial.print("b2: ");
      Serial.print(data.b2);
      Serial.print(";");

      Serial.print("gb: ");
      Serial.print(data.gb);
      Serial.print(";");

      Serial.print("rb: ");
      Serial.print(data.rb);
      Serial.print(";");

      Serial.println();
    }
  }

  /**
     Set the data
  */
  // Left Switch
  data.s1 = digitalRead(leftSwitch);

  // Left Pot
  data.p1 = map(adc.analogRead(leftPot), 0, 1023, 0, 255);

  // Left Joystick
  joystickRead(analogRead(leftJoystickUpDown), 2);
  data.j1u = joystick.val1;
  data.j1d = joystick.val2;

  joystickRead(analogRead(leftJoystickLeftRight), 2);
  data.j1l = joystick.val2;
  data.j1r = joystick.val1;

  data.j1b = digitalRead(leftJoystickButton); // Button

  // Left Thumb Joystick
  joystickRead(adc.analogRead(leftThumbUpDown), 5);
  data.tj1u = joystick.val2;
  data.tj1d = joystick.val1;

  joystickRead(adc.analogRead(leftThumbLeftRight), 5);
  data.tj1l = joystick.val1;
  data.tj1r = joystick.val2;

  // Left Btn
  data.b1 = digitalRead(leftButton);

  // Right Switch
  data.s2 = digitalRead(rightSwitch);

  // Right Pot
  data.p2 = map(adc.analogRead(rightPot), 0, 1023, 0, 255);

  // Right Joystick
  joystickRead(analogRead(rightJoystickUpDown), 2);
  data.j2u = joystick.val1;
  data.j2d = joystick.val2;

  joystickRead(analogRead(rightJoystickLeftRight), 3);
  data.j2l = joystick.val2;
  data.j2r = joystick.val1;
  data.j2b = digitalRead(rightJoystickButton); // Button

  // Right Thumb Joystick
  joystickRead(adc.analogRead(rightThumbUpDown), 5);
  data.tj2u = joystick.val1;
  data.tj2d = joystick.val2;

  joystickRead(adc.analogRead(rightThumbLeftRight), 5);
  data.tj2l = joystick.val1;
  data.tj2r = joystick.val2;

  // Right Button
  data.b2 = digitalRead(rightButton);

  // Rotary Encoder
  // data.re;

  // Rotary Encoder - Push
  data.rep = digitalRead(rotaryEncoderPush);

  // Green Button
  data.gb = digitalRead(button1);

  // Red Button
  data.rb = digitalRead(button2);

  // RC transmitter time
  data.transmitterTime = millis();

  // Send the whole data from the structure to the receiver
  radioTx.write(&data, sizeof(package));
}

void defaultValues() {
  // Left Switch
  data.s1 = 1;

  // Left Pot
  data.p1 = 127;

  // Left Joystick
  data.j1u = 127; // Up
  data.j1d = 127; // Down
  data.j1l = 127; // Left
  data.j1r = 127; // Right
  data.j1b = 1; // Button

  // Left Thumb Joystick
  data.tj1u = 127; // Up
  data.tj1d = 127; // Down
  data.tj1l = 127; // Left
  data.tj1r = 127; // Right

  // Left Btn
  data.b1 = 1;

  // Right Switch
  data.s2 = 1;

  // Right Pot
  data.p2 = 127;

  // Right Joystick
  data.j2u = 127; // Up
  data.j2d = 127; // Down
  data.j2l = 127; // Left
  data.j2r = 127; // Right
  data.j2b = 1; // Button

  // Right Thumb Joystick
  data.tj2u = 127; // Up
  data.tj2d = 127; // Down
  data.tj2l = 127; // Left
  data.tj2r = 127; // Right

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
}


unsigned long testText() {
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Hello World!");
}
