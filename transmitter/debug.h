/**
   Usefull to debug the buttons
*/
void serialDebug(package data) {
  Serial.print("v: ");
  Serial.print(analogRead(VOLTAGE_PIN));
  Serial.print(";");

  Serial.print("v: ");
  Serial.print(voltageTX);
  Serial.print(";");

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
