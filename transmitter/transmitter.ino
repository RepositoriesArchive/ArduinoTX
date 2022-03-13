/**
  Libraries
*/
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h> // Required for "radio.printDetails();"
#include <MCP3XXX.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <EEPROM.h>
#include "config.h"
#include "util.h"
#include "debug.h"

/**
   MCP3008 Init
*/
MCP3008 adc;

/**
   TFT Init
*/
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
#include "tft_init.h"

/**
   RF24 Init
*/
RF24 radioRx(RX_CE, RX_CSN);
RF24 radioTx(TX_CE, TX_CSN);

void setup() {
  /**
     Set the default values
  */
  defaultValues();

  /* Serial Port */
  if (debug)
    Serial.begin(115200);

  /**
    radioTx Init
  */
  radioTx.begin();
  radioTx.setPALevel(RF24_PA_MIN);  // Reduce Power Consumption
  radioTx.setDataRate(RF24_250KBPS);  // Reduce Power Consumption
  radioTx.setChannel(TX_CHANNEL);  // 2.508 Ghz - Above most Wifi Channels
  radioTx.openWritingPipe(addressTx);
  radioTx.stopListening();

  if (debug) {
    printf_begin();
    radioTx.printDetails();
    Serial.print("TX Connection Status: ");
    Serial.println(radioTx.isChipConnected());
  }

  /**
     TX Status
  */
  if (radioTx.isChipConnected())
    TX_STATUS = true;

  /**
     radioRx Set-Up
  */
  radioRx.begin();
  radioRx.setPALevel(RF24_PA_MIN);  // Reduce Power Consumption
  radioRx.setDataRate(RF24_250KBPS);  // Reduce Power Consumption
  radioRx.setChannel(RX_CHANNEL);  // 2.508 Ghz - Above most Wifi Channels
  radioRx.openReadingPipe(0, addressRx);
  radioRx.startListening();

  if (debug) {
    printf_begin();
    radioRx.printDetails();
    Serial.print("RX Connection Status: ");
    Serial.println(radioRx.isChipConnected());
  }

  /**
     RX Status
  */
  if (radioRx.isChipConnected())
    RX_STATUS = true;

  /**
     Activate the Arduino internal pull-up resistors
  */
  pinMode(VOLTAGE_PIN, INPUT);
  pinMode(8, INPUT);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);

  /**
     MCP3008 Init
  */
  adc.begin();

  /**
     TFT INIT
  */
  pinMode(TFT_LCD, OUTPUT);
  analogWrite(TFT_LCD, 40);

  tft.begin();
  tft.fillScreen(BLACK);
  tft.setRotation(45);

  tftHomepage();
}


/**
   TFT Update
*/
#include "tft_update.h"

void loop() {
  /**
     Read the voltage
  */
  voltageTX = analogRead(VOLTAGE_PIN) * (5 / 1023.00);

  /* Transmission Message */
  if (radioRx.available()) {
    radioRx.read(&dataRx, sizeof(packageRx));

    /**
       If data received, update the lastReceiveTime
    */
    lastReceiveTime = millis();

    /**
       If current time is more then .5 second since we have recived the last data, that means we have lost connection
    */
    if ( currentTime - lastReceiveTime > 500 ) {
      /**
         Either use the default data or adjust to some safe values based on your device
      */
      defaultValues();
    }
  }

  /**
     Read the data
  */
  // Left Switch
  data.s1 = digitalRead(leftSwitch);

  // Left Pot
  data.p1 = map(adc.analogRead(leftPot), 0, 1023, 0, 255);

  // Left Joystick
  joystickRead(analogRead(leftJoystickUpDown), 2);
  if (leftJoystickReverseUpDown) {
    data.j1u = joystick.val2;
    data.j1d = joystick.val1;
  } else {
    data.j1u = joystick.val1;
    data.j1d = joystick.val2;
  }

  joystickRead(analogRead(leftJoystickLeftRight), 2);
  if (leftJoystickReverseLeftRight) {
    data.j1l = joystick.val2;
    data.j1r = joystick.val1;
  } else {
    data.j1l = joystick.val1;
    data.j1r = joystick.val2;
  }

  data.j1b = digitalRead(leftJoystickButton); // Button

  // Left Thumb Joystick
  joystickRead(adc.analogRead(leftThumbUpDown), 5);
  if (leftThumbReverseUpDown) {
    data.tj1u = joystick.val2;
    data.tj1d = joystick.val1;
  } else {
    data.tj1u = joystick.val1;
    data.tj1d = joystick.val2;
  }

  joystickRead(adc.analogRead(leftThumbLeftRight), 5);
  if (leftThumbReverseLeftRight) {
    data.tj1l = joystick.val2;
    data.tj1r = joystick.val1;
  } else {
    data.tj1l = joystick.val1;
    data.tj1r = joystick.val2;
  }

  // Left Btn
  data.b1 = digitalRead(leftButton);

  // Right Switch
  data.s2 = digitalRead(rightSwitch);

  // Right Pot
  data.p2 = map(adc.analogRead(rightPot), 0, 1023, 0, 255);

  // Right Joystick
  joystickRead(analogRead(rightJoystickUpDown), 2);
  if (rightJoystickReverseUpDown) {
    data.j2u = joystick.val2;
    data.j2d = joystick.val1;
  } else {
    data.j2u = joystick.val1;
    data.j2d = joystick.val2;
  }

  joystickRead(analogRead(rightJoystickLeftRight), 3);
  if (rightJoystickReverseLeftRight) {
    data.j2l = joystick.val2;
    data.j2r = joystick.val1;
  } else {
    data.j2l = joystick.val1;
    data.j2r = joystick.val2;
  }
  data.j2b = digitalRead(rightJoystickButton); // Button

  // Right Thumb Joystick
  joystickRead(adc.analogRead(rightThumbUpDown), 5);
  if (rightThumbReverseUpDown) {
    data.tj2u = joystick.val2;
    data.tj2d = joystick.val1;
  } else {
    data.tj2u = joystick.val1;
    data.tj2d = joystick.val2;
  }

  joystickRead(adc.analogRead(rightThumbLeftRight), 5);
  if (rightThumbReverseLeftRight) {
    data.tj2l = joystick.val2;
    data.tj2r = joystick.val1;
  } else {
    data.tj2l = joystick.val1;
    data.tj2r = joystick.val2;
  }

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

  if (debug)
    serialDebug(data);

  // Send the whole data from the structure to the receiver
  radioTx.write(&data, sizeof(package));

  lcdUpdate(data, voltageTX, dataRx);
}
