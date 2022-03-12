/**
  Libraries
*/
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h> // Required for "radio.printDetails();"
#include <MCP3XXX.h>
#include <Adafruit_GFX.h>       // include Adafruit graphics library
#include <Adafruit_ILI9341.h>   // include Adafruit ILI9341 TFT library
#include <EEPROM.h> //  To save data on EEPROM
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

// Colours
#define SKY     0x667F // nice light blue
#define WHITE   0xFFFF
#define GREY    0xE73C
#define ORANGE  0xFBA0
#define GREEN   0x2685
#define BLACK   0x0000

// initialize ILI9341 TFT library
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
String CurrentPage;
String PrevPage;
int StatusBarColor = SKY;
int MenuColor;
int TextColor =  WHITE;

/**
   nRF24L01
*/
#define RX_CE 13
#define RX_CSN 14
#define RX_CHANNEL 108  // 2.508 Ghz - Above most Wifi Channels
bool RX_STATUS;

#define TX_CE 23
#define TX_CSN 22
#define TX_CHANNEL 108  // 2.508 Ghz - Above most Wifi Channels
bool TX_STATUS;

//TX Setup
const byte address[6] = "00001";
const char text[] = "Hello World";
char textRx[32] = "";

RF24 radioRx(RX_CE, RX_CSN);
RF24 radioTx(TX_CE, TX_CSN);

/** -------- */

float voltage;
#define VOLTAGE_PIN 27

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

// Store the current values
struct packageRx {
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
packageRx dataRx;

unsigned long currentMillis = millis();

void setup() {
  /**
     Set the default values
  */
  defaultValues();

  if (debug) {
    /* Serial Port */
    Serial.begin(115200);
  }

  /**
    radioTx Init
  */
  radioTx.begin();
  radioTx.setPALevel(RF24_PA_MIN);  // Reduce Power Consumption
  radioTx.setDataRate(RF24_250KBPS);  // Reduce Power Consumption
  radioTx.setChannel(TX_CHANNEL);  // 2.508 Ghz - Above most Wifi Channels
  radioTx.openWritingPipe(address);
  radioTx.stopListening();

  if (debug) {
    printf_begin();
    radioTx.printDetails();
    Serial.print("TX Connection Status: ");
    Serial.println(radioTx.isChipConnected());
  }
  if (radioTx.isChipConnected()) {
    TX_STATUS = true;
  }

  /**
     radioRx Set-Up
  */
  radioRx.begin();
  radioRx.setPALevel(RF24_PA_MIN);  // Reduce Power Consumption
  radioRx.setDataRate(RF24_250KBPS);  // Reduce Power Consumption
  radioRx.setChannel(RX_CHANNEL);  // 2.508 Ghz - Above most Wifi Channels
  radioRx.openReadingPipe(0, address);
  radioRx.startListening();

  if (debug) {
    printf_begin();
    radioRx.printDetails();
    Serial.print("RX Connection Status: ");
    Serial.println(radioRx.isChipConnected());
  }
  if (radioRx.isChipConnected()) {
    RX_STATUS = true;
  }

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

  // MCP3008 Init
  adc.begin();

  // TFT INIT
  pinMode(TFT_LCD, OUTPUT);
  analogWrite(TFT_LCD, 40);//11

  tft.begin();
  tft.fillScreen(BLACK);
  tft.setRotation(45);

  tftHomepage();

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
  voltage = analogRead(VOLTAGE_PIN);
  voltage = voltage * (5 / 1023.00);

  /* Transmission Message */
  if (radioRx.available()) {
    radioRx.read(&dataRx, sizeof(packageRx));
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
      Serial.print("v: ");
      Serial.print(analogRead(VOLTAGE_PIN));
      Serial.print(";");

      Serial.print("v: ");
      Serial.print(voltage);
      Serial.print(";");

      Serial.print("s1: ");
      Serial.print(dataRx.s1);
      Serial.print(";");

      Serial.print("p1: ");
      Serial.print(dataRx.p1);
      Serial.print(";");

      Serial.print("j1ud: ");
      Serial.print(dataRx.j1u);
      Serial.print(":");
      Serial.print(dataRx.j1d);
      Serial.print(";");

      Serial.print("j1lr: ");
      Serial.print(dataRx.j1l);
      Serial.print(":");
      Serial.print(dataRx.j1r);
      Serial.print(";");

      Serial.print("j1b: ");
      Serial.print(dataRx.j1b);
      Serial.print(";");

      Serial.print("tj1ud: ");
      Serial.print(dataRx.tj1u);
      Serial.print(":");
      Serial.print(dataRx.tj1d);
      Serial.print(";");

      Serial.print("tj1lr: ");
      Serial.print(dataRx.tj1l);
      Serial.print(":");
      Serial.print(dataRx.tj1r);
      Serial.print(";");

      Serial.print("b1: ");
      Serial.print(dataRx.b1);
      Serial.print(";");

      Serial.print("s2: ");
      Serial.print(dataRx.s2);
      Serial.print(";");

      Serial.print("p2: ");
      Serial.print(dataRx.p2);
      Serial.print(";");

      Serial.print("j2u: ");
      Serial.print(dataRx.j2u);
      Serial.print(":");
      Serial.print(dataRx.j2d);
      Serial.print(";");

      Serial.print("j2l: ");
      Serial.print(dataRx.j2l);
      Serial.print(":");
      Serial.print(dataRx.j2r);
      Serial.print(";");

      Serial.print("j2b: ");
      Serial.print(dataRx.j2b);
      Serial.print(";");

      Serial.print("tj2u: ");
      Serial.print(dataRx.tj2u);
      Serial.print(":");
      Serial.print(dataRx.tj2d);
      Serial.print(";");

      Serial.print("tj2l: ");
      Serial.print(dataRx.tj2l);
      Serial.print(":");
      Serial.print(dataRx.tj2r);
      Serial.print(";");

      Serial.print("b2: ");
      Serial.print(dataRx.b2);
      Serial.print(";");

      Serial.print("gb: ");
      Serial.print(dataRx.gb);
      Serial.print(";");

      Serial.print("rb: ");
      Serial.print(dataRx.rb);
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

  lcdUpdate(data.j1u, data.j1d, data.j2u, data.j2d, data.j1l, data.j1r, data.j2l, data.j2r, voltage);
}

void lcdUpdate(
  int var1, int var2, int var3, int var4, int var5, int var6, int var7, int var8, float voltage
) {
  // LCD Update
  tft.setTextSize(7);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(18, 50);

  tft.print(addDigits(var1, var2, 3));
  tft.setTextColor(GREY, BLACK);
  tft.print(" ");
  tft.setTextColor(WHITE, BLACK);
  tft.print(addDigits(var3, var4, 3));

  /*  LEFT/RIGHT  */
  tft.setTextSize(7);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(18, 110);
  tft.print(addDigits(var5, var6, 3));
  tft.setTextColor(GREY, BLACK);
  tft.print(" ");
  tft.setTextColor(WHITE, BLACK);
  tft.print(addDigits(var7, var8, 3));


  /*  Voltage Tx  */
  tft.setTextSize(1);
  tft.setTextColor(GREY, BLACK);
  tft.setCursor(50, 195);
  tft.print("Voltage TX: ");
  tft.setTextColor(SKY, BLACK);
  tft.print(valRound(voltage));
  tft.print("v");

  tft.print("  ");     //  Separator

  /*  Voltage Rx  */
  tft.setTextSize(1);
  tft.setCursor(180, 195);
  tft.setTextColor(GREY);
  tft.print("Voltage RX: ");
  tft.setTextColor(SKY);
  tft.print("0.0");
  tft.print("v");
}

/**
   Set the default values
*/
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

  // RX & TX Status
  RX_STATUS = false;
  TX_STATUS = false;
}

/**
   TFT Pages
*/
void tftHomepage()  {
  CurrentPage     =   "Homepage";
  StatusBarColor  =   BLACK;
  MenuColor       =   StatusBarColor;
  TextColor       =   TextColor;
  drawDashBoard();  //  Draw DashBoard
  PrevPage        =   CurrentPage;    //  Reset Previous Page variable
}

/**
   Dashboard Items
*/
void drawDashBoard()  {
  //  tft.setCursor(110, 45);
  tft.setCursor(80, 7);
  tft.setTextSize(3);
  tft.setTextColor(GREY);
  tft.println("ArduinoTX");

  /*  RX Status  */
  tft.setTextSize(1);
  tft.setTextColor(GREY);
  tft.setCursor(50, 180);
  tft.print("RX Status: ");
  if (RX_STATUS) {
    tft.setTextColor(GREEN);
    tft.print("ON");
  } else {
    tft.setTextColor(ORANGE);
    tft.print("OFF");
  }

  tft.print("  ");     //  Separator

  /*  TX Status  */
  tft.setCursor(180, 180);
  tft.setTextColor(GREY);
  tft.print("TX Status: ");
  tft.setTextColor(SKY);
  if (TX_STATUS) {
    tft.setTextColor(GREEN);
    tft.print("ON");
  } else {
    tft.setTextColor(ORANGE);
    tft.print("OFF");
  }

}

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

float valRound(float val) {
  return round(val * 100.0) / 100.0;
}
