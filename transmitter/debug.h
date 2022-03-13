/**
 * Usefull to debug the TX - RX
 */

void serialDebug() {
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
