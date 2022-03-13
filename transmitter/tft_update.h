/**
   Update the info data
*/
float vTX;
float vRX;
int p1;
int p2;

void lcdUpdate(
  package data, float voltage, packageRx dataRx
) {

  if ( (abs(p1 - data.p1) > 2) or (abs(p2 - data.p2) > 2) ) {
    tft.setTextSize(7);
    tft.setTextColor(WHITE, BLACK);
    tft.setCursor(18, 70);

    tft.print(addDigits(data.p1, 0, 3));
    tft.setTextColor(GREY, BLACK);
    tft.print(" ");
    tft.setTextColor(WHITE, BLACK);
    tft.print(addDigits(data.p2, 0, 3));
    p1 = data.p1;
    p2 = data.p2;
  }

  if (abs(vTX - voltage) > 0.25 or abs(vRX - dataRx.voltage) > 0.25) {
    /*  Voltage Tx  */
    tft.setTextSize(1.4);
    tft.setTextColor(GREY, BLACK);
    tft.setCursor(50, 195);
    tft.print("Voltage TX: ");
    tft.setTextColor(SKY, BLACK);
    tft.print(valRound(voltage));
    tft.print("v");
    vTX = voltage;

    tft.print("  ");     //  Separator

    /*  Voltage Rx  */
    tft.setCursor(180, 195);
    tft.setTextColor(GREY, BLACK);
    tft.print("Voltage RX: ");
    tft.setTextColor(SKY, BLACK);
    if (dataRx.voltage > 1) {
      tft.print(valRound(dataRx.voltage));
    } else {
      tft.print("0.00");
    }
    tft.print("v");
    vRX = dataRx.voltage;
  }

}
