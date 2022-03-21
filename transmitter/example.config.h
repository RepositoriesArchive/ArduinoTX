/**
   Enable/Disable the debug mode
*/

bool debug = false;
bool debug_rx = false;
bool debug_tx = false;

/**
   Colours
*/
#define SKY     0x667F // nice light blue
#define WHITE   0xFFFF
#define GREY    0xE73C
#define ORANGE  0xFBA0
#define GREEN   0x2685
#define BLACK   0x0000

/**
   Define the current PINS - Read the buttons
*/
// Left Switch
#define leftSwitch A4

// Left Pot
#define leftPot 6

// Left Joystick
bool leftJoystickReverseUpDown = false;
#define leftJoystickUpDown 24

bool leftJoystickReverseLeftRight = true;
#define leftJoystickLeftRight 25
#define leftJoystickButton 21

// Left Thumb Joystick
bool leftThumbReverseUpDown = true;
#define leftThumbUpDown 0

bool leftThumbReverseLeftRight = false;
#define leftThumbLeftRight 1

// Left Button
#define leftButton 17

// Right Switch
#define rightSwitch A5

// Right Pot
#define rightPot 4

// Right Joystick
bool rightJoystickReverseUpDown = false;
#define rightJoystickUpDown 30

bool rightJoystickReverseLeftRight = true;
#define rightJoystickLeftRight 31

#define rightJoystickButton 16

// Right Thumb Joystick
bool rightThumbReverseUpDown = false;
#define rightThumbUpDown 2

bool rightThumbReverseLeftRight = false;
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

float voltageTX;
#define VOLTAGE_PIN 27

/**
   MCP3008 pin connections
*/
#define CS_PIN 4
#define CLOCK_PIN 7
#define MOSI_PIN 5
#define MISO_PIN 6

/**
   TFT
*/
#define TFT_RST   26
#define TFT_DC    0
#define TFT_CS    1
#define TFT_LCD   12

// TFT - Menu/Pages
String CurrentPage;
String PrevPage;
int StatusBarColor = SKY;
int MenuColor;
int TextColor = WHITE;

/**
   nRF24L01+
*/
#define RX_CE 13
#define RX_CSN 14
#define RX_CHANNEL 105
bool RX_STATUS;


#define TX_CE 23
#define TX_CSN 22
#define TX_CHANNEL 106
bool TX_STATUS;

//TX Setup
uint8_t address[][6] = {"A1", "B1", "A2", "B2"};
char textRx[32] = "";

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
package data;

// Store the current values
struct packageRx {
  byte voltage;
  byte voltage_2;
};

// Create a variable with the above structure
packageRx dataRx;
