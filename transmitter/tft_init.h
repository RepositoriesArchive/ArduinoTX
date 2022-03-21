/**
   Dashboard Items
*/
void drawDashBoard()  {
  tft.setCursor(80, 7);
  tft.setTextSize(3);
  tft.setTextColor(GREY);
  tft.println("ArduinoTX");

  // LCD Update
  tft.setTextSize(7);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(18, 70);

  tft.print("000");
  tft.setTextColor(GREY, BLACK);
  tft.print(" ");
  tft.setTextColor(WHITE, BLACK);
  tft.print("000");

  /*  Voltage Tx  */
  tft.setTextSize(1.4);
  tft.setTextColor(GREY, BLACK);
  tft.setCursor(50, 195);
  tft.print("Voltage TX: ");
  tft.setTextColor(SKY, BLACK);
  tft.print("0.00");
  tft.print("v");

  tft.print("  ");     //  Separator

  /*  Voltage Rx  */
  tft.setCursor(180, 195);
  tft.setTextColor(GREY, BLACK);
  tft.print("Voltage RX: ");
  tft.setTextColor(SKY, BLACK);
  tft.print("0.00");
  tft.print("v");

  /*  RX Status  */
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

  tft.print("  "); // Separator

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


/**
   TFT Homepage
*/
void tftHomepage()
{
  CurrentPage     =   "Homepage";
  StatusBarColor  =   BLACK;
  MenuColor       =   StatusBarColor;
  TextColor       =   TextColor;
  drawDashBoard();  //  Draw DashBoard
  PrevPage        =   CurrentPage;
}
